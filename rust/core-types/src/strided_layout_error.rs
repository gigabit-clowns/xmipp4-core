//! Typed errors returned by strided layout operations.

use crate::slice_error::SliceError;
use std::error::Error;
use std::fmt::{Display, Formatter};

/// Errors produced by [`crate::StridedLayout`] methods.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum StridedLayoutError {
	RankMismatch,
	InvalidAxisPermutationLength,
	MissingAxisInPermutation {
		index: usize,
	},
	ReverseIndexOutOfBounds {
		index: isize,
		extent: usize,
	},
	IndexOutOfBounds {
		index: isize,
		extent: usize,
	},
	AxesMustDiffer,
	BroadcastRankMismatch {
		rank: usize,
		target_rank: usize,
	},
	AxisNotBroadcastable {
		axis_extent: usize,
		target_extent: usize,
	},
	MultipleEllipsis,
	NoMoreAxesForIndex,
	NoMoreAxesForSlice,
	InvalidSlice(SliceError),
}

impl Display for StridedLayoutError {
	fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
		match self {
			StridedLayoutError::RankMismatch => {
				write!(f, "extents and strides must have the same length")
			}
			StridedLayoutError::InvalidAxisPermutationLength => {
				write!(
					f,
					"Axis permutation's length does not match the required count"
				)
			}
			StridedLayoutError::MissingAxisInPermutation { index } => {
				write!(f, "Index {} is missing in the axis permutation", index)
			}
			StridedLayoutError::ReverseIndexOutOfBounds { index, extent } => {
				write!(
					f,
					"Reverse index {} is out of bounds for extent {}",
					index, extent
				)
			}
			StridedLayoutError::IndexOutOfBounds { index, extent } => {
				write!(f, "Index {} is out of bounds for extent {}", index, extent)
			}
			StridedLayoutError::AxesMustDiffer => {
				write!(f, "axis1 and axis2 must represent different axes")
			}
			StridedLayoutError::BroadcastRankMismatch { rank, target_rank } => write!(
				f,
				"Cannot broadcast layout with {} axes into a shape of {} dimensions.",
				rank, target_rank
			),
			StridedLayoutError::AxisNotBroadcastable {
				axis_extent,
				target_extent,
			} => write!(
				f,
				"Cannot broadcast axis of extent {} into an extent of {}.",
				axis_extent, target_extent
			),
			StridedLayoutError::MultipleEllipsis => write!(
				f,
				"Two ellipsis tags were encountered when processing subscripts"
			),
			StridedLayoutError::NoMoreAxesForIndex => write!(
				f,
				"An index subscript was encountered, but there are no more axes to process"
			),
			StridedLayoutError::NoMoreAxesForSlice => write!(
				f,
				"A slice subscript was encountered, but there are no more axes to process"
			),
			StridedLayoutError::InvalidSlice(error) => Display::fmt(error, f),
		}
	}
}

impl Error for StridedLayoutError {
	fn source(&self) -> Option<&(dyn Error + 'static)> {
		match self {
			StridedLayoutError::InvalidSlice(error) => Some(error),
			_ => None,
		}
	}
}

impl From<SliceError> for StridedLayoutError {
	fn from(value: SliceError) -> Self {
		StridedLayoutError::InvalidSlice(value)
	}
}
