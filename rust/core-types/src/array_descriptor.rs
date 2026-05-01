use crate::{NumericalType, StridedLayout};

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct ArrayDescriptor {
	layout: StridedLayout,
	data_type: NumericalType,
}

impl Default for ArrayDescriptor {
	fn default() -> Self {
		Self {
			layout: StridedLayout::default(),
			data_type: NumericalType::Unknown,
		}
	}
}

impl ArrayDescriptor {
	pub fn new(layout: StridedLayout, data_type: NumericalType) -> Self {
		Self { layout, data_type }
	}

	pub fn layout(&self) -> &StridedLayout {
		&self.layout
	}

	pub fn data_type(&self) -> NumericalType {
		self.data_type
	}
}

pub fn is_initialized(descriptor: &ArrayDescriptor) -> bool {
	descriptor.data_type() != NumericalType::Unknown && descriptor.layout().rank() > 0
}

pub fn compute_storage_requirement(descriptor: &ArrayDescriptor) -> usize {
	let bytes_per_element = match descriptor.data_type().size_bytes() {
		Some(value) => value,
		None => return 0,
	};

	descriptor
		.layout()
		.compute_storage_requirement()
		.saturating_mul(bytes_per_element)
}
