// SPDX-License-Identifier: GPL-3.0-only

//! Typed errors returned by strided layout operations.

use crate::slice_error::SliceError;
use thiserror::Error;

/// Errors produced by [`crate::StridedLayout`] methods.
#[derive(Debug, Error, Clone, PartialEq, Eq)]
pub enum StridedLayoutError {
	#[error("extents and strides must have the same length (expected {expected}, got {actual})")]
	RankMismatch { expected: usize, actual: usize },
	#[error("Axis permutation's length does not match the required count")]
	InvalidAxisPermutationLength,
	#[error("Index {index} is missing in the axis permutation")]
	MissingAxisInPermutation { index: usize },
	#[error("Reverse index {index} is out of bounds for extent {extent}")]
	ReverseIndexOutOfBounds { index: isize, extent: usize },
	#[error("Index {index} is out of bounds for extent {extent}")]
	IndexOutOfBounds { index: isize, extent: usize },
	#[error("axis1 and axis2 must represent different axes (got axis1={axis1}, axis2={axis2})")]
	AxesMustDiffer { axis1: isize, axis2: isize },
	#[error("Cannot broadcast layout with {rank} axes into a shape of {target_rank} dimensions.")]
	BroadcastRankMismatch { rank: usize, target_rank: usize },
	#[error("Cannot broadcast axis of extent {axis_extent} into an extent of {target_extent}.")]
	AxisNotBroadcastable {
		axis_extent: usize,
		target_extent: usize,
	},
	#[error("Two ellipsis tags were encountered when processing subscripts")]
	MultipleEllipsis,
	#[error("An index subscript was encountered, but there are no more axes to process")]
	NoMoreAxesForIndex,
	#[error("A slice subscript was encountered, but there are no more axes to process")]
	NoMoreAxesForSlice,
	#[error(transparent)]
	InvalidSlice(#[from] SliceError),
}
