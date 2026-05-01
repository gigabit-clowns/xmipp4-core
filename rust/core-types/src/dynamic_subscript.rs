// SPDX-License-Identifier: GPL-3.0-only

//! Dynamic subscript model.

use crate::slice::Slice;

/// Runtime subscript representation used by [`crate::StridedLayout::apply_subscripts`].
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum DynamicSubscript {
	Ellipsis,
	NewAxis,
	Index(isize),
	Slice(Slice),
}

/// Returns an ellipsis subscript.
pub fn ellipsis() -> DynamicSubscript {
	DynamicSubscript::Ellipsis
}

/// Returns a new-axis subscript.
pub fn new_axis() -> DynamicSubscript {
	DynamicSubscript::NewAxis
}