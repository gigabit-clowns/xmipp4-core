// SPDX-License-Identifier: GPL-3.0-only

use xmipp4_core_types::subscript::{make_slice, make_slice_full, make_slice_start, sanitize_slice};
use xmipp4_core_types::SliceError;

#[test]
fn sanitize_slice_rejects_zero_step() {
	let error = sanitize_slice(make_slice_full(0, 3, 0), 10).expect_err("must fail");
	assert_eq!(error, SliceError::StepZero);
}

#[test]
fn sanitize_slice_rejects_negative_start_out_of_bounds() {
	let error = sanitize_slice(make_slice_start(-6, 1), 5).expect_err("must fail");
	assert_eq!(
		error,
		SliceError::NegativeStartOutOfBounds {
			start: -6,
			extent: 5,
		}
	);
}

#[test]
fn sanitize_slice_rejects_positive_start_out_of_bounds() {
	let error = sanitize_slice(make_slice_start(6, 1), 5).expect_err("must fail");
	assert_eq!(
		error,
		SliceError::StartOutOfBounds {
			start: 6,
			extent: 5,
		}
	);
}

#[test]
fn sanitize_slice_rejects_backward_start_out_of_bounds() {
	let error = sanitize_slice(make_slice_full(5, 1, -1), 5).expect_err("must fail");
	assert_eq!(
		error,
		SliceError::BackwardStartOutOfBounds {
			start: 5,
			extent: 5,
		}
	);
}

#[test]
fn sanitize_slice_rejects_forward_overflow() {
	let error = sanitize_slice(make_slice_full(1, 5, 1), 5).expect_err("must fail");
	assert_eq!(
		error,
		SliceError::PositiveSliceOverflowsExtent {
			count: 5,
			start: 1,
			step: 1,
			extent: 5,
		}
	);
}

#[test]
fn sanitize_slice_rejects_backward_underflow() {
	let error = sanitize_slice(make_slice_full(2, 4, -1), 5).expect_err("must fail");
	assert_eq!(
		error,
		SliceError::ReversedSliceUnderflowsZero {
			count: 4,
			start: 2,
			step: -1,
		}
	);
}

#[test]
fn sanitize_slice_normalizes_negative_start() {
	let slice = sanitize_slice(make_slice_start(-2, 2), 8).expect("must pass");
	assert_eq!(slice.start(), 6);
	assert_eq!(slice.count(), 2);
	assert_eq!(slice.step(), 1);
}

#[test]
fn sanitize_slice_expands_unbounded_forward_count() {
	let slice = sanitize_slice(make_slice(usize::MAX), 7).expect("must pass");
	assert_eq!(slice.start(), 0);
	assert_eq!(slice.count(), 7);
	assert_eq!(slice.step(), 1);
}
