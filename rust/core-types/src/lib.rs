// SPDX-License-Identifier: GPL-3.0-only

//! Core public types and helpers used by the Rust parity layer for Xmipp4.
//!
//! This crate mirrors selected C++ concepts with stable public names so the
//! API can evolve toward drop-in replacement goals.

/// Array descriptor module.
pub mod array_descriptor;
mod c_api;
/// Dynamic subscript model.
pub mod dynamic_subscript;
/// Numerical type system and promotion rules.
pub mod numerical_type;
/// Runtime slice model and normalization helpers.
pub mod slice;
/// Typed errors produced by slice sanitization.
pub mod slice_error;
/// Strided layout model and shape/stride transforms.
pub mod strided_layout;
/// Typed errors produced by strided layout operations.
pub mod strided_layout_error;
/// Dynamic subscripts and slice helpers.
pub mod subscript;

pub use array_descriptor::{compute_storage_requirement, is_initialized, ArrayDescriptor};
pub use dynamic_subscript::DynamicSubscript;
pub use numerical_type::{promote_types, NumericalType, NumericalTypeCategory};
pub use slice::Slice;
pub use slice_error::SliceError;
pub use strided_layout::StridedLayout;
pub use strided_layout_error::StridedLayoutError;
