//! Core public types and helpers used by the Rust parity layer for Xmipp4.
//!
//! This crate mirrors selected C++ concepts with stable public names so the
//! API can evolve toward drop-in replacement goals.

/// Array descriptor module.
pub mod array_descriptor;
/// Numerical type system and promotion rules.
pub mod numerical_type;
/// Typed errors produced by slice sanitization.
pub mod slice_error;
/// Strided layout model and shape/stride transforms.
pub mod strided_layout;
/// Typed errors produced by strided layout operations.
pub mod strided_layout_error;
/// Dynamic subscripts and slice helpers.
pub mod subscript;

pub use array_descriptor::{compute_storage_requirement, is_initialized, ArrayDescriptor};
pub use numerical_type::{promote_types, NumericalType, NumericalTypeCategory};
pub use slice_error::SliceError;
pub use strided_layout::StridedLayout;
pub use strided_layout_error::StridedLayoutError;
pub use subscript::{DynamicSubscript, Slice};
