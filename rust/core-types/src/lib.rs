pub mod array_descriptor;
pub mod numerical_type;
pub mod slice_error;
pub mod strided_layout;
pub mod strided_layout_error;
pub mod subscript;

pub use array_descriptor::{compute_storage_requirement, is_initialized, ArrayDescriptor};
pub use numerical_type::{promote_types, NumericalType, NumericalTypeCategory};
pub use slice_error::SliceError;
pub use strided_layout::StridedLayout;
pub use strided_layout_error::StridedLayoutError;
pub use subscript::{
	all, ellipsis, end, even, make_slice, make_slice_full, make_slice_start, new_axis, odd,
	DynamicSubscript, Slice,
};
