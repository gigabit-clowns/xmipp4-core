pub mod array_descriptor;
pub mod numerical_type;
pub mod strided_layout;
pub mod subscript;

pub use array_descriptor::{compute_storage_requirement, is_initialized, ArrayDescriptor};
pub use numerical_type::{NumericalType, NumericalTypeCategory};
pub use strided_layout::StridedLayout;
pub use subscript::{
	all, ellipsis, end, even, make_slice, make_slice_full, make_slice_start, new_axis, odd,
	DynamicSubscript, Slice,
};
