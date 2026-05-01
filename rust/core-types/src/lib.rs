pub mod array_descriptor;
pub mod numerical_type;
pub mod strided_layout;

pub use array_descriptor::{compute_storage_requirement, is_initialized, ArrayDescriptor};
pub use numerical_type::{NumericalType, NumericalTypeCategory};
pub use strided_layout::StridedLayout;
