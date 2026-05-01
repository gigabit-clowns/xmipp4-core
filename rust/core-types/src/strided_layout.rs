#[derive(Debug, Clone, PartialEq, Eq, Hash, Default)]
pub struct StridedLayout {
	extents: Vec<usize>,
	strides: Vec<isize>,
	offset: isize,
}

impl StridedLayout {
	pub fn new(extents: Vec<usize>, strides: Vec<isize>, offset: isize) -> Result<Self, String> {
		if extents.len() != strides.len() {
			return Err("extents and strides must have the same length".to_string());
		}

		Ok(Self {
			extents,
			strides,
			offset,
		})
	}

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

	pub fn rank(&self) -> usize {
		self.extents.len()
	}

	pub fn extents(&self) -> &[usize] {
		&self.extents
	}

	pub fn strides(&self) -> &[isize] {
		&self.strides
	}

	pub fn offset(&self) -> isize {
		self.offset
	}

	pub fn extents_equal(&self, extents: &[usize]) -> bool {
		self.extents == extents
	}

	pub fn compute_element_count(&self) -> usize {
		if self.extents.is_empty() {
			return 0;
		}

		self.extents.iter().product()
	}

	pub fn compute_storage_requirement(&self) -> usize {
		if self.extents.is_empty() || self.extents.contains(&0) {
			return 0;
		}

		let mut min_index = self.offset;
		let mut max_index = self.offset;

		for (extent, stride) in self.extents.iter().zip(self.strides.iter()) {
			let delta = (*extent as isize - 1) * *stride;
			if delta >= 0 {
				max_index += delta;
			} else {
				min_index += delta;
			}
		}

		if max_index < min_index {
			0
		} else {
			(max_index - min_index + 1) as usize
		}
	}

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

	pub fn permute(&self, order: &[usize]) -> Result<Self, String> {
		check_axis_permutation(order, self.rank())?;

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

	pub fn matrix_transpose(&self, axis1: isize, axis2: isize) -> Result<Self, String> {
		let rank = self.rank();
		let index1 = sanitize_index(axis1, rank)?;
		let index2 = sanitize_index(axis2, rank)?;

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

	pub fn matrix_diagonal(&self, axis1: isize, axis2: isize) -> Result<Self, String> {
		let rank = self.rank();
		let mut index1 = sanitize_index(axis1, rank)?;
		let mut index2 = sanitize_index(axis2, rank)?;

		if axis1 == axis2 {
			return Err("axis1 and axis2 must represent different axes".to_string());
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

	pub fn broadcast_to(&self, extents: &[usize]) -> Result<Self, String> {
		if self.extents_equal(extents) {
			return Ok(self.clone());
		}

		if self.rank() > extents.len() {
			return Err(format!(
				"Cannot broadcast layout with {} axes into a shape of {} dimensions.",
				self.rank(),
				extents.len()
			));
		}

		let padding = extents.len() - self.rank();
		let mut out_extents = Vec::with_capacity(extents.len());
		let mut out_strides = Vec::with_capacity(extents.len());

		for &extent in extents.iter().take(padding) {
			out_extents.push(extent);
			out_strides.push(0);
		}

		for (&extent, &stride) in self.extents.iter().zip(self.strides.iter()) {
			out_extents.push(extent);
			out_strides.push(stride);
		}

		for i in 0..extents.len() {
			let target_extent = extents[i];
			let axis_extent = out_extents[i];
			if axis_extent != target_extent {
				if axis_extent == 1 {
					out_extents[i] = target_extent;
					out_strides[i] = 0;
				} else {
					return Err(format!(
						"Cannot broadcast axis of extent {} into an extent of {}.",
						axis_extent, target_extent
					));
				}
			}
		}

		Ok(Self {
			extents: out_extents,
			strides: out_strides,
			offset: self.offset,
		})
	}
}

fn check_axis_permutation(order: &[usize], count: usize) -> Result<(), String> {
	if order.len() != count {
		return Err("Axis permutation's length does not match the required count".to_string());
	}

	for expected in 0..count {
		if !order.contains(&expected) {
			return Err(format!(
				"Index {} is missing in the axis permutation",
				expected
			));
		}
	}

	Ok(())
}

fn sanitize_index(index: isize, extent: usize) -> Result<usize, String> {
	if index < 0 {
		if index < -(extent as isize) {
			return Err(format!(
				"Reverse index {} is out of bounds for extent {}",
				index, extent
			));
		}

		return Ok((index + extent as isize) as usize);
	}

	if index >= extent as isize {
		return Err(format!(
			"Index {} is out of bounds for extent {}",
			index, extent
		));
	}

	Ok(index as usize)
}
