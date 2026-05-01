// SPDX-License-Identifier: GPL-3.0-only

//! Runtime slice model and slice sanitization utilities.

use crate::slice_error::SliceError;

/// Runtime slice descriptor.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct Slice {
	start: isize,
	count: usize,
	step: isize,
}

/// Sentinel value used to represent unbounded slice count.
pub fn end() -> usize {
	usize::MAX
}

/// Returns a full forward slice (`0..` with step `1`).
pub fn all() -> Slice {
	Slice::new(0, end(), 1)
}

/// Returns a forward slice selecting even positions.
pub fn even() -> Slice {
	Slice::new(0, end(), 2)
}

/// Returns a forward slice selecting odd positions.
pub fn odd() -> Slice {
	Slice::new(1, end(), 2)
}

/// Returns a slice starting at `0` with the provided count and step `1`.
pub fn make_slice(count: usize) -> Slice {
	Slice::new(0, count, 1)
}

/// Returns a slice with custom start and count and step `1`.
pub fn make_slice_start(start: isize, count: usize) -> Slice {
	Slice::new(start, count, 1)
}

/// Returns a slice with custom start, count and step.
pub fn make_slice_full(start: isize, count: usize, step: isize) -> Slice {
	Slice::new(start, count, step)
}

impl Slice {
	/// Creates a slice from raw start, count and step fields.
	pub fn new(start: isize, count: usize, step: isize) -> Self {
		Self { start, count, step }
	}

	/// Returns slice start index.
	pub fn start(self) -> isize {
		self.start
	}

	/// Returns slice element count.
	pub fn count(self) -> usize {
		self.count
	}

	/// Returns slice step.
	pub fn step(self) -> isize {
		self.step
	}
}

/// Validates and normalizes a slice against an axis extent.
///
/// # Errors
///
/// Returns a typed error when slice step, start, or effective count is
/// incompatible with the provided extent.
pub fn sanitize_slice(slice: Slice, extent: usize) -> Result<Slice, SliceError> {
	validate_non_zero_slice_step(slice)?;
	let slice = normalize_slice_start(slice, extent)?;
	normalize_slice_count(slice, extent)
}

fn validate_non_zero_slice_step(slice: Slice) -> Result<(), SliceError> {
	if slice.step == 0 {
		return Err(SliceError::StepZero);
	}

	Ok(())
}

fn normalize_slice_start(mut slice: Slice, extent: usize) -> Result<Slice, SliceError> {
	let start = slice.start;
	let count = slice.count;
	let step = slice.step;

	validate_slice_start_lower_bound(start, extent)?;
	validate_slice_start_upper_bound(start, count, step, extent)?;
	remap_negative_slice_start(&mut slice, extent);

	Ok(slice)
}

fn validate_slice_start_lower_bound(start: isize, extent: usize) -> Result<(), SliceError> {
	if start < -(extent as isize) {
		return Err(SliceError::NegativeStartOutOfBounds { start, extent });
	}

	Ok(())
}

fn validate_slice_start_upper_bound(
	start: isize,
	count: usize,
	step: isize,
	extent: usize,
) -> Result<(), SliceError> {
	if step > 0 || count == 0 {
		if start > extent as isize {
			return Err(SliceError::StartOutOfBounds { start, extent });
		}
		return Ok(());
	}

	if start >= extent as isize {
		return Err(SliceError::BackwardStartOutOfBounds { start, extent });
	}

	Ok(())
}

fn remap_negative_slice_start(slice: &mut Slice, extent: usize) {
	if slice.start < 0 {
		slice.start += extent as isize;
	}
}

fn normalize_slice_count(mut slice: Slice, extent: usize) -> Result<Slice, SliceError> {
	let start = slice.start;
	let count = slice.count;
	let step = slice.step;

	if step > 0 {
		normalize_forward_slice_count(&mut slice, extent, start, count, step)?;
	} else {
		normalize_backward_slice_count(&mut slice, start, count, step)?;
	}

	Ok(slice)
}

fn normalize_forward_slice_count(
	slice: &mut Slice,
	extent: usize,
	start: isize,
	count: usize,
	step: isize,
) -> Result<(), SliceError> {
	if count == end() {
		slice.count = (extent - start as usize).div_ceil(step as usize);
		return Ok(());
	}

	if count > 0 && start + step * (count as isize - 1) >= extent as isize {
		return Err(SliceError::PositiveSliceOverflowsExtent {
			count,
			start,
			step,
			extent,
		});
	}

	Ok(())
}

fn normalize_backward_slice_count(
	slice: &mut Slice,
	start: isize,
	count: usize,
	step: isize,
) -> Result<(), SliceError> {
	let abs_step = -step;
	if count == end() {
		slice.count = start as usize / abs_step as usize + 1;
		return Ok(());
	}

	if count > 0 && abs_step * (count as isize - 1) > start {
		return Err(SliceError::ReversedSliceUnderflowsZero { count, start, step });
	}

	Ok(())
}
