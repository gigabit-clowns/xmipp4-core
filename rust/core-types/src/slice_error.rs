// SPDX-License-Identifier: GPL-3.0-only

//! Typed errors returned by slice sanitization.

use std::error::Error;
use std::fmt::{Display, Formatter};

/// Errors produced when normalizing a [`crate::slice::Slice`].
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum SliceError {
	StepZero,
	NegativeStartOutOfBounds {
		start: isize,
		extent: usize,
	},
	StartOutOfBounds {
		start: isize,
		extent: usize,
	},
	BackwardStartOutOfBounds {
		start: isize,
		extent: usize,
	},
	PositiveSliceOverflowsExtent {
		count: usize,
		start: isize,
		step: isize,
		extent: usize,
	},
	ReversedSliceUnderflowsZero {
		count: usize,
		start: isize,
		step: isize,
	},
}

impl Display for SliceError {
	fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
		match self {
			SliceError::StepZero => write!(f, "Slice step cannot be zero."),
			SliceError::NegativeStartOutOfBounds { start, extent } => write!(
				f,
				"Slice's negative start index {} is out of bounds for extent {}.",
				start, extent
			),
			SliceError::StartOutOfBounds { start, extent } => write!(
				f,
				"Slice's start index {} is out of bounds for extent {}.",
				start, extent
			),
			SliceError::BackwardStartOutOfBounds { start, extent } => write!(
				f,
				"Backwards slice's start index {} is out of bounds for extent {}.",
				start, extent
			),
			SliceError::PositiveSliceOverflowsExtent {
				count,
				start,
				step,
				extent,
			} => write!(
				f,
				"Slice count {} start index {} and step {} overflows extent {}",
				count, start, step, extent
			),
			SliceError::ReversedSliceUnderflowsZero { count, start, step } => write!(
				f,
				"Reversed slice with count {} start index {} and step {} underflows 0",
				count, start, step
			),
		}
	}
}

impl Error for SliceError {}
