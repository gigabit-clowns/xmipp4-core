// SPDX-License-Identifier: GPL-3.0-only

//! Strided layout model and axis-transform operations.

use crate::strided_layout_error::StridedLayoutError;
use crate::subscript::{sanitize_slice, DynamicSubscript, Slice};
use std::hash::{Hash, Hasher};

/// Describes an n-dimensional memory layout using extents, strides, and offset.
#[derive(Debug, Clone, Default)]
pub struct StridedLayout {
	extents: Vec<usize>,
	strides: Vec<isize>,
	offset: isize,
}

impl PartialEq for StridedLayout {
	fn eq(&self, other: &Self) -> bool {
		if self.offset != other.offset {
			return false;
		}

		if self.extents.len() != other.extents.len() {
			return false;
		}

		for i in 0..self.extents.len() {
			let extent = self.extents[i];
			if extent != other.extents[i] {
				return false;
			}

			if extent != 1 && self.strides[i] != other.strides[i] {
				return false;
			}
		}

		true
	}
}

impl Eq for StridedLayout {}

impl Hash for StridedLayout {
	fn hash<H: Hasher>(&self, state: &mut H) {
		self.offset.hash(state);
		for i in 0..self.extents.len() {
			let extent = self.extents[i];
			extent.hash(state);

			let stride = if extent == 1 { 0 } else { self.strides[i] };
			stride.hash(state);
		}
	}
}

impl StridedLayout {
	/// Creates a layout from extents, strides and base offset.
	///
	/// # Errors
	///
	/// Returns an error when extents and strides have different ranks.
	pub fn new(
		extents: Vec<usize>,
		strides: Vec<isize>,
		offset: isize,
	) -> Result<Self, StridedLayoutError> {
		if extents.len() != strides.len() {
			return Err(StridedLayoutError::RankMismatch);
		}

		Ok(Self {
			extents,
			strides,
			offset,
		})
	}

	/// Builds a contiguous row-major layout for the provided extents.
	pub fn make_contiguous_layout(extents: Vec<usize>) -> Self {
		let rank = extents.len();
		if rank == 0 {
			return Self::default();
		}

		let mut strides = vec![0isize; rank];
		let mut stride = 1isize;
		for idx in (0..rank).rev() {
			strides[idx] = stride;
			stride = stride.saturating_mul(extents[idx] as isize);
		}

		Self {
			extents,
			strides,
			offset: 0,
		}
	}

	/// Returns the number of axes.
	pub fn rank(&self) -> usize {
		self.extents.len()
	}

	/// Returns axis extents.
	pub fn extents(&self) -> &[usize] {
		&self.extents
	}

	/// Returns axis strides.
	pub fn strides(&self) -> &[isize] {
		&self.strides
	}

	/// Returns the storage offset.
	pub fn offset(&self) -> isize {
		self.offset
	}

	/// Checks whether extents exactly match the provided shape.
	pub fn extents_equal(&self, extents: &[usize]) -> bool {
		self.extents == extents
	}

	/// Returns total logical element count.
	pub fn compute_element_count(&self) -> usize {
		if self.extents.is_empty() {
			return 0;
		}

		self.extents.iter().product()
	}

	/// Computes storage span requirement using C++-aligned semantics.
	pub fn compute_storage_requirement(&self) -> usize {
		let mut result = 0usize;

		for (&extent, &stride) in self.extents.iter().zip(self.strides.iter()) {
			if extent == 0 {
				return 0;
			}

			if stride > 0 {
				let last_index = extent - 1;
				result = result.wrapping_add(last_index.wrapping_mul(stride as usize));
			}
		}

		(self.offset.wrapping_add(result as isize).wrapping_add(1)) as usize
	}

	/// Reverses axis order.
	pub fn transpose(&self) -> Self {
		if self.rank() <= 1 {
			return self.clone();
		}

		let mut extents = self.extents.clone();
		extents.reverse();

		let mut strides = self.strides.clone();
		strides.reverse();

		Self {
			extents,
			strides,
			offset: self.offset,
		}
	}

	/// Permutes axes according to the provided order.
	///
	/// # Errors
	///
	/// Returns an error when order is not a valid permutation of all axes.
	pub fn permute(&self, order: &[usize]) -> Result<Self, StridedLayoutError> {
		validate_axis_permutation(order, self.rank())?;

		let mut extents = Vec::with_capacity(order.len());
		let mut strides = Vec::with_capacity(order.len());
		for &axis in order {
			extents.push(self.extents[axis]);
			strides.push(self.strides[axis]);
		}

		Ok(Self {
			extents,
			strides,
			offset: self.offset,
		})
	}

	/// Swaps two axes by index (supports negative indices).
	///
	/// # Errors
	///
	/// Returns an error when an axis index is out of bounds.
	pub fn matrix_transpose(&self, axis1: isize, axis2: isize) -> Result<Self, StridedLayoutError> {
		let rank = self.rank();
		let index1 = normalize_axis_index(axis1, rank)?;
		let index2 = normalize_axis_index(axis2, rank)?;

		let mut extents = self.extents.clone();
		extents.swap(index1, index2);

		let mut strides = self.strides.clone();
		strides.swap(index1, index2);

		Ok(Self {
			extents,
			strides,
			offset: self.offset,
		})
	}

	/// Extracts a diagonal view from two axes.
	///
	/// # Errors
	///
	/// Returns an error when indices are invalid or axes are identical.
	pub fn matrix_diagonal(&self, axis1: isize, axis2: isize) -> Result<Self, StridedLayoutError> {
		let rank = self.rank();
		let mut index1 = normalize_axis_index(axis1, rank)?;
		let mut index2 = normalize_axis_index(axis2, rank)?;

		if axis1 == axis2 {
			return Err(StridedLayoutError::AxesMustDiffer);
		}

		if index1 > index2 {
			std::mem::swap(&mut index1, &mut index2);
		}

		let mut extents = Vec::with_capacity(rank.saturating_sub(1));
		let mut strides = Vec::with_capacity(rank.saturating_sub(1));

		for i in 0..rank {
			if i != index1 && i != index2 {
				extents.push(self.extents[i]);
				strides.push(self.strides[i]);
			}
		}

		let diag_extent = self.extents[index1].min(self.extents[index2]);
		let diag_stride = self.strides[index1] + self.strides[index2];
		extents.push(diag_extent);
		strides.push(diag_stride);

		Ok(Self {
			extents,
			strides,
			offset: self.offset,
		})
	}

	/// Removes axes with extent equal to one.
	pub fn squeeze(&self) -> Self {
		let mut extents = Vec::new();
		let mut strides = Vec::new();

		for (&extent, &stride) in self.extents.iter().zip(self.strides.iter()) {
			if extent != 1 {
				extents.push(extent);
				strides.push(stride);
			}
		}

		Self {
			extents,
			strides,
			offset: self.offset,
		}
	}

	/// Broadcasts this layout to the target extents.
	///
	/// # Errors
	///
	/// Returns an error when broadcasting is not possible.
	pub fn broadcast_to(&self, extents: &[usize]) -> Result<Self, StridedLayoutError> {
		if self.extents_equal(extents) {
			return Ok(self.clone());
		}

		let left_padding_axis_count = compute_broadcast_padding(self.rank(), extents.len())?;
		let (mut out_extents, mut out_strides) =
			initialize_broadcast_axes(self, extents, left_padding_axis_count);
		apply_broadcast_adjustments(extents, &mut out_extents, &mut out_strides)?;

		Ok(Self {
			extents: out_extents,
			strides: out_strides,
			offset: self.offset,
		})
	}

	/// Applies dynamic subscripts and returns the resulting layout view.
	///
	/// # Errors
	///
	/// Returns an error for invalid subscripts, out-of-range accesses, or
	/// incompatible slice/index combinations.
	pub fn apply_subscripts(
		&self,
		subscripts: &[DynamicSubscript],
	) -> Result<Self, StridedLayoutError> {
		if subscripts.is_empty() {
			return Ok(self.clone());
		}

		let mut state = SubscriptTraversalState::new(self.offset, self.rank());
		let ellipsis_pos = find_ellipsis_position(subscripts)?;
		apply_subscript_partitions(self, subscripts, ellipsis_pos, &mut state)?;

		Ok(assemble_subscripted_layout(self, state))
	}
}

struct SubscriptTraversalState {
	offset: isize,
	left_axis: usize,
	right_axis: usize,
	left_output_extents: Vec<usize>,
	left_output_strides: Vec<isize>,
	right_output_extents: Vec<usize>,
	right_output_strides: Vec<isize>,
}

impl SubscriptTraversalState {
	fn new(offset: isize, rank: usize) -> Self {
		Self {
			offset,
			left_axis: 0,
			right_axis: rank,
			left_output_extents: Vec::new(),
			left_output_strides: Vec::new(),
			right_output_extents: Vec::new(),
			right_output_strides: Vec::new(),
		}
	}
}

fn find_ellipsis_position(
	subscripts: &[DynamicSubscript],
) -> Result<Option<usize>, StridedLayoutError> {
	let mut ellipsis_pos = None;

	for (index, subscript) in subscripts.iter().enumerate() {
		if !matches!(subscript, DynamicSubscript::Ellipsis) {
			continue;
		}

		if ellipsis_pos.is_some() {
			return Err(StridedLayoutError::MultipleEllipsis);
		}

		ellipsis_pos = Some(index);
	}

	Ok(ellipsis_pos)
}

fn apply_subscript_partitions(
	layout: &StridedLayout,
	subscripts: &[DynamicSubscript],
	ellipsis_pos: Option<usize>,
	state: &mut SubscriptTraversalState,
) -> Result<(), StridedLayoutError> {
	if let Some(ellipsis_pos) = ellipsis_pos {
		apply_leading_subscripts(layout, &subscripts[..ellipsis_pos], state)?;
		apply_trailing_subscripts(layout, &subscripts[ellipsis_pos + 1..], state)?;
		return Ok(());
	}

	apply_leading_subscripts(layout, subscripts, state)
}

fn apply_leading_subscripts(
	layout: &StridedLayout,
	subscripts: &[DynamicSubscript],
	state: &mut SubscriptTraversalState,
) -> Result<(), StridedLayoutError> {
	for subscript in subscripts {
		apply_subscript_forward(
			layout,
			subscript,
			&mut state.left_axis,
			state.right_axis,
			&mut state.offset,
			&mut state.left_output_extents,
			&mut state.left_output_strides,
		)?;
	}

	Ok(())
}

fn apply_trailing_subscripts(
	layout: &StridedLayout,
	subscripts: &[DynamicSubscript],
	state: &mut SubscriptTraversalState,
) -> Result<(), StridedLayoutError> {
	for subscript in subscripts.iter().rev() {
		apply_subscript_backward(
			layout,
			subscript,
			state.left_axis,
			&mut state.right_axis,
			&mut state.offset,
			&mut state.right_output_extents,
			&mut state.right_output_strides,
		)?;
	}

	Ok(())
}

fn assemble_subscripted_layout(
	layout: &StridedLayout,
	mut state: SubscriptTraversalState,
) -> StridedLayout {
	let mut extents = Vec::new();
	let mut strides = Vec::new();

	extents.extend_from_slice(&state.left_output_extents);
	strides.extend_from_slice(&state.left_output_strides);

	for i in state.left_axis..state.right_axis {
		extents.push(layout.extents[i]);
		strides.push(layout.strides[i]);
	}

	state.right_output_extents.reverse();
	state.right_output_strides.reverse();
	extents.extend_from_slice(&state.right_output_extents);
	strides.extend_from_slice(&state.right_output_strides);

	StridedLayout {
		extents,
		strides,
		offset: state.offset,
	}
}

fn validate_axis_permutation(order: &[usize], count: usize) -> Result<(), StridedLayoutError> {
	if order.len() != count {
		return Err(StridedLayoutError::InvalidAxisPermutationLength);
	}

	for expected in 0..count {
		if !order.contains(&expected) {
			return Err(StridedLayoutError::MissingAxisInPermutation { index: expected });
		}
	}

	Ok(())
}

fn normalize_axis_index(index: isize, extent: usize) -> Result<usize, StridedLayoutError> {
	if index < 0 {
		if index < -(extent as isize) {
			return Err(StridedLayoutError::ReverseIndexOutOfBounds { index, extent });
		}

		return Ok((index + extent as isize) as usize);
	}

	if index >= extent as isize {
		return Err(StridedLayoutError::IndexOutOfBounds { index, extent });
	}

	Ok(index as usize)
}

fn compute_broadcast_padding(rank: usize, target_rank: usize) -> Result<usize, StridedLayoutError> {
	if rank > target_rank {
		return Err(StridedLayoutError::BroadcastRankMismatch { rank, target_rank });
	}

	Ok(target_rank - rank)
}

fn initialize_broadcast_axes(
	layout: &StridedLayout,
	target_extents: &[usize],
	padding: usize,
) -> (Vec<usize>, Vec<isize>) {
	let mut out_extents = Vec::with_capacity(target_extents.len());
	let mut out_strides = Vec::with_capacity(target_extents.len());

	for &extent in target_extents.iter().take(padding) {
		out_extents.push(extent);
		out_strides.push(0);
	}

	for (&extent, &stride) in layout.extents.iter().zip(layout.strides.iter()) {
		out_extents.push(extent);
		out_strides.push(stride);
	}

	(out_extents, out_strides)
}

fn apply_broadcast_adjustments(
	target_extents: &[usize],
	out_extents: &mut [usize],
	out_strides: &mut [isize],
) -> Result<(), StridedLayoutError> {
	for i in 0..target_extents.len() {
		let target_extent = target_extents[i];
		let axis_extent = out_extents[i];
		if axis_extent == target_extent {
			continue;
		}

		if axis_extent != 1 {
			return Err(StridedLayoutError::AxisNotBroadcastable {
				axis_extent,
				target_extent,
			});
		}

		out_extents[i] = target_extent;
		out_strides[i] = 0;
	}

	Ok(())
}

fn apply_subscript_forward(
	layout: &StridedLayout,
	subscript: &DynamicSubscript,
	left_axis: &mut usize,
	right_axis: usize,
	offset: &mut isize,
	out_extents: &mut Vec<usize>,
	out_strides: &mut Vec<isize>,
) -> Result<(), StridedLayoutError> {
	match subscript {
		DynamicSubscript::Ellipsis => Ok(()),
		DynamicSubscript::NewAxis => {
			out_extents.push(1);
			out_strides.push(0);
			Ok(())
		}
		DynamicSubscript::Index(index) => {
			ensure_axis_available_for_index(*left_axis, right_axis)?;
			apply_index(layout, *left_axis, *index, offset)?;
			*left_axis += 1;
			Ok(())
		}
		DynamicSubscript::Slice(slice) => {
			ensure_axis_available_for_slice(*left_axis, right_axis)?;
			let (extent, stride) = apply_slice(layout, *left_axis, *slice, offset)?;
			out_extents.push(extent);
			out_strides.push(stride);
			*left_axis += 1;
			Ok(())
		}
	}
}

fn apply_subscript_backward(
	layout: &StridedLayout,
	subscript: &DynamicSubscript,
	left_axis: usize,
	right_axis: &mut usize,
	offset: &mut isize,
	out_extents: &mut Vec<usize>,
	out_strides: &mut Vec<isize>,
) -> Result<(), StridedLayoutError> {
	match subscript {
		DynamicSubscript::Ellipsis => Err(StridedLayoutError::MultipleEllipsis),
		DynamicSubscript::NewAxis => {
			out_extents.push(1);
			out_strides.push(0);
			Ok(())
		}
		DynamicSubscript::Index(index) => {
			ensure_axis_available_for_index(left_axis, *right_axis)?;
			let axis_index = *right_axis - 1;
			apply_index(layout, axis_index, *index, offset)?;
			*right_axis -= 1;
			Ok(())
		}
		DynamicSubscript::Slice(slice) => {
			ensure_axis_available_for_slice(left_axis, *right_axis)?;
			let axis_index = *right_axis - 1;
			let (extent, stride) = apply_slice(layout, axis_index, *slice, offset)?;
			out_extents.push(extent);
			out_strides.push(stride);
			*right_axis -= 1;
			Ok(())
		}
	}
}

fn ensure_axis_available_for_index(
	left_axis: usize,
	right_axis: usize,
) -> Result<(), StridedLayoutError> {
	if left_axis == right_axis {
		return Err(StridedLayoutError::NoMoreAxesForIndex);
	}

	Ok(())
}

fn ensure_axis_available_for_slice(
	left_axis: usize,
	right_axis: usize,
) -> Result<(), StridedLayoutError> {
	if left_axis == right_axis {
		return Err(StridedLayoutError::NoMoreAxesForSlice);
	}

	Ok(())
}

fn apply_index(
	layout: &StridedLayout,
	axis_index: usize,
	index: isize,
	offset: &mut isize,
) -> Result<(), StridedLayoutError> {
	let sanitized_index = normalize_axis_index(index, layout.extents[axis_index])?;
	*offset += sanitized_index as isize * layout.strides[axis_index];
	Ok(())
}

fn apply_slice(
	layout: &StridedLayout,
	axis_index: usize,
	slice: Slice,
	offset: &mut isize,
) -> Result<(usize, isize), StridedLayoutError> {
	let sanitized_slice = sanitize_slice(slice, layout.extents[axis_index])?;
	*offset += layout.strides[axis_index] * sanitized_slice.start();
	let new_extent = sanitized_slice.count();
	let new_stride = layout.strides[axis_index] * sanitized_slice.step();
	Ok((new_extent, new_stride))
}
