// SPDX-License-Identifier: GPL-3.0-only

//! Array descriptor type that couples a layout with its numerical data type.

use crate::{NumericalType, StridedLayout};

/// Describes an n-dimensional array through layout and element type metadata.
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
	/// Builds a descriptor from a strided layout and numerical type.
	pub fn new(layout: StridedLayout, data_type: NumericalType) -> Self {
		Self { layout, data_type }
	}

	/// Returns the array layout metadata.
	pub fn layout(&self) -> &StridedLayout {
		&self.layout
	}

	/// Returns the numerical type of each array element.
	pub fn data_type(&self) -> NumericalType {
		self.data_type
	}
}

/// Returns whether the descriptor has a known, concrete numerical type.
pub fn is_initialized(descriptor: &ArrayDescriptor) -> bool {
	descriptor.data_type() != NumericalType::Unknown
}

/// Computes total byte storage required by the descriptor.
///
/// Returns `0` when the type size is unknown.
pub fn compute_storage_requirement(descriptor: &ArrayDescriptor) -> usize {
	descriptor.layout().compute_storage_requirement()
		* descriptor.data_type().size_bytes().unwrap_or(0)
}
