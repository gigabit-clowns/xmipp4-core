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
}
