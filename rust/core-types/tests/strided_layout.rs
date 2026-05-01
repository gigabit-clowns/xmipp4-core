use std::collections::hash_map::DefaultHasher;
use std::hash::{Hash, Hasher};
use xmipp4_core_types::subscript::{all, ellipsis, even, make_slice, new_axis, odd};
use xmipp4_core_types::{DynamicSubscript, SliceError, StridedLayout, StridedLayoutError};

fn make_test_layout() -> StridedLayout {
	StridedLayout::new(
		vec![120, 56, 24, 1, 10, 8],
		vec![860_160, 7_680, 320, 160, 16, 2],
		20,
	)
	.expect("valid layout")
}

#[test]
fn default_layout_is_empty() {
	let layout = StridedLayout::default();
	assert_eq!(layout.rank(), 0);
	assert_eq!(layout.compute_element_count(), 0);
	assert_eq!(layout.compute_storage_requirement(), 1);
}

#[test]
fn contiguous_layout_has_expected_strides() {
	let layout = StridedLayout::make_contiguous_layout(vec![3, 4, 5]);
	assert_eq!(layout.extents(), &[3, 4, 5]);
	assert_eq!(layout.strides(), &[20, 5, 1]);
	assert_eq!(layout.compute_element_count(), 60);
	assert_eq!(layout.compute_storage_requirement(), 60);
}

#[test]
fn custom_layout_with_negative_stride_reports_required_span() {
	let layout = StridedLayout::new(vec![2, 3], vec![-3, 1], 5).expect("valid layout");
	assert_eq!(layout.compute_storage_requirement(), 8);
}

#[test]
fn equality_ignores_stride_for_extent_one_axes() {
	let a = StridedLayout::new(vec![4, 1, 5], vec![5, 111, 1], 3).expect("valid");
	let b = StridedLayout::new(vec![4, 1, 5], vec![5, -777, 1], 3).expect("valid");

	assert_eq!(a, b);
}

#[test]
fn hash_is_equal_when_only_extent_one_stride_changes() {
	let a = StridedLayout::new(vec![4, 1, 5], vec![5, 111, 1], 3).expect("valid");
	let b = StridedLayout::new(vec![4, 1, 5], vec![5, -777, 1], 3).expect("valid");

	let mut hasher_a = DefaultHasher::new();
	a.hash(&mut hasher_a);

	let mut hasher_b = DefaultHasher::new();
	b.hash(&mut hasher_b);

	assert_eq!(hasher_a.finish(), hasher_b.finish());
}

#[test]
fn constructor_rejects_mismatched_rank() {
	let error = StridedLayout::new(vec![2, 2], vec![1], 0).expect_err("must fail");
	assert_eq!(error, StridedLayoutError::RankMismatch);
}

#[test]
fn transpose_reverses_axes_and_preserves_offset() {
	let layout = make_test_layout();
	let transposed = layout.transpose();

	assert_eq!(transposed.extents(), &[8, 10, 1, 24, 56, 120]);
	assert_eq!(transposed.strides(), &[2, 16, 160, 320, 7_680, 860_160]);
	assert_eq!(transposed.offset(), layout.offset());
}

#[test]
fn transpose_on_default_layout_returns_empty_layout() {
	let layout = StridedLayout::default();
	let transposed = layout.transpose();

	assert_eq!(transposed, StridedLayout::default());
}

#[test]
fn permute_with_valid_permutation_reorders_axes() {
	let layout = make_test_layout();
	let permuted = layout
		.permute(&[0, 3, 1, 2, 4, 5])
		.expect("valid permutation");

	assert_eq!(permuted.extents(), &[120, 1, 56, 24, 10, 8]);
	assert_eq!(permuted.strides(), &[860_160, 160, 7_680, 320, 16, 2]);
	assert_eq!(permuted.offset(), layout.offset());
}

#[test]
fn permute_with_non_permutation_fails_with_expected_message() {
	let layout = make_test_layout();
	let error = layout.permute(&[0, 1, 2, 3, 4, 4]).expect_err("must fail");

	assert_eq!(
		error,
		StridedLayoutError::MissingAxisInPermutation { index: 5 }
	);
}

#[test]
fn permute_on_default_layout_with_empty_permutation_succeeds() {
	let layout = StridedLayout::default();
	let permuted = layout.permute(&[]).expect("empty permutation should pass");

	assert_eq!(permuted, StridedLayout::default());
}

#[test]
fn permute_on_default_layout_with_non_empty_permutation_fails() {
	let layout = StridedLayout::default();
	let error = layout.permute(&[0]).expect_err("must fail");

	assert_eq!(error, StridedLayoutError::InvalidAxisPermutationLength);
}

#[test]
fn matrix_transpose_swaps_requested_axes() {
	let layout = make_test_layout();
	let swapped = layout.matrix_transpose(1, 2).expect("valid axes");

	assert_eq!(swapped.extents(), &[120, 24, 56, 1, 10, 8]);
	assert_eq!(swapped.strides(), &[860_160, 320, 7_680, 160, 16, 2]);
	assert_eq!(swapped.offset(), layout.offset());
}

#[test]
fn matrix_transpose_fails_when_axis_is_out_of_bounds() {
	let layout = make_test_layout();

	let error = layout
		.matrix_transpose(6, 0)
		.expect_err("axis must be out of bounds");
	assert_eq!(
		error,
		StridedLayoutError::IndexOutOfBounds {
			index: 6,
			extent: 6,
		}
	);

	let error = layout
		.matrix_transpose(0, 6)
		.expect_err("axis must be out of bounds");
	assert_eq!(
		error,
		StridedLayoutError::IndexOutOfBounds {
			index: 6,
			extent: 6,
		}
	);
}

#[test]
fn matrix_transpose_on_default_layout_always_fails() {
	let layout = StridedLayout::default();

	let error = layout
		.matrix_transpose(0, 0)
		.expect_err("default layout must fail");
	assert_eq!(
		error,
		StridedLayoutError::IndexOutOfBounds {
			index: 0,
			extent: 0,
		}
	);

	let error = layout
		.matrix_transpose(1, 1)
		.expect_err("default layout must fail");
	assert_eq!(
		error,
		StridedLayoutError::IndexOutOfBounds {
			index: 1,
			extent: 0,
		}
	);
}

#[test]
fn squeeze_removes_axes_of_extent_one() {
	let layout = make_test_layout();
	let squeezed = layout.squeeze();

	assert_eq!(squeezed.extents(), &[120, 56, 24, 10, 8]);
	assert_eq!(squeezed.strides(), &[860_160, 7_680, 320, 16, 2]);
	assert_eq!(squeezed.offset(), 20);
}

#[test]
fn squeeze_on_default_layout_returns_empty_layout() {
	let layout = StridedLayout::default();
	let squeezed = layout.squeeze();

	assert_eq!(squeezed, StridedLayout::default());
}

#[test]
fn matrix_diagonal_returns_layout_with_diagonal_elements() {
	let layout = make_test_layout();
	let diagonal = layout.matrix_diagonal(-2, 1).expect("valid axes");

	assert_eq!(diagonal.extents(), &[120, 24, 1, 8, 10]);
	assert_eq!(diagonal.strides(), &[860_160, 320, 160, 2, 7_680 + 16]);
	assert_eq!(diagonal.offset(), layout.offset());
}

#[test]
fn matrix_diagonal_fails_when_axis_is_out_of_bounds() {
	let layout = make_test_layout();

	let error = layout
		.matrix_diagonal(6, 0)
		.expect_err("axis must be out of bounds");
	assert_eq!(
		error,
		StridedLayoutError::IndexOutOfBounds {
			index: 6,
			extent: 6,
		}
	);

	let error = layout
		.matrix_diagonal(0, 6)
		.expect_err("axis must be out of bounds");
	assert_eq!(
		error,
		StridedLayoutError::IndexOutOfBounds {
			index: 6,
			extent: 6,
		}
	);
}

#[test]
fn matrix_diagonal_on_default_layout_always_fails() {
	let layout = StridedLayout::default();

	let error = layout
		.matrix_diagonal(0, 0)
		.expect_err("default layout must fail");
	assert_eq!(
		error,
		StridedLayoutError::IndexOutOfBounds {
			index: 0,
			extent: 0,
		}
	);

	let error = layout
		.matrix_diagonal(1, 0)
		.expect_err("default layout must fail");
	assert_eq!(
		error,
		StridedLayoutError::IndexOutOfBounds {
			index: 1,
			extent: 0,
		}
	);
}

#[test]
fn matrix_diagonal_requires_different_axes() {
	let layout = make_test_layout();
	let error = layout.matrix_diagonal(0, 0).expect_err("must fail");

	assert_eq!(error, StridedLayoutError::AxesMustDiffer);
}

#[test]
fn broadcast_to_fills_left_and_promotes_extent_one_axes() {
	let layout = make_test_layout();
	let target_extents = vec![16, 40, 120, 56, 24, 9, 10, 8];

	let broadcasted = layout
		.broadcast_to(&target_extents)
		.expect("valid broadcast");

	assert_eq!(broadcasted.extents(), target_extents);
	assert_eq!(
		broadcasted.strides(),
		&[0, 0, 860_160, 7_680, 320, 0, 16, 2]
	);
	assert_eq!(broadcasted.offset(), 20);
}

#[test]
fn broadcast_to_on_default_layout_succeeds() {
	let layout = StridedLayout::default();
	let target_extents = vec![120, 56, 24, 1, 10, 8];

	let broadcasted = layout
		.broadcast_to(&target_extents)
		.expect("broadcast should succeed");

	assert_eq!(broadcasted.extents(), target_extents);
	assert_eq!(broadcasted.strides(), &[0, 0, 0, 0, 0, 0]);
	assert_eq!(broadcasted.offset(), 0);
}

#[test]
fn broadcast_to_fails_when_target_has_fewer_axes() {
	let layout = make_test_layout();
	let target_extents = vec![56, 24, 1, 10, 8];

	let error = layout.broadcast_to(&target_extents).expect_err("must fail");
	assert_eq!(
		error,
		StridedLayoutError::BroadcastRankMismatch {
			rank: 6,
			target_rank: 5,
		}
	);
}

#[test]
fn broadcast_to_fails_when_axis_is_not_broadcastable() {
	let layout = make_test_layout();
	let target_extents = vec![120, 55, 24, 1, 10, 8];

	let error = layout.broadcast_to(&target_extents).expect_err("must fail");
	assert_eq!(
		error,
		StridedLayoutError::AxisNotBroadcastable {
			axis_extent: 56,
			target_extent: 55,
		}
	);
}

#[test]
fn apply_subscripts_implicitly_fills_remaining_axes() {
	let layout = make_test_layout();
	let subscripts = vec![DynamicSubscript::Index(1)];

	let result = layout
		.apply_subscripts(&subscripts)
		.expect("subscripts must be valid");

	assert_eq!(result.extents(), &[56, 24, 1, 10, 8]);
	assert_eq!(result.strides(), &[7_680, 320, 160, 16, 2]);
	assert_eq!(result.offset(), layout.offset() + 860_160);
}

#[test]
fn apply_subscripts_implicitly_fills_axes_after_ellipsis() {
	let layout = make_test_layout();
	let subscripts = vec![ellipsis(), DynamicSubscript::Index(1)];

	let result = layout
		.apply_subscripts(&subscripts)
		.expect("subscripts must be valid");

	assert_eq!(result.extents(), &[120, 56, 24, 1, 10]);
	assert_eq!(result.strides(), &[860_160, 7_680, 320, 160, 16]);
	assert_eq!(result.offset(), layout.offset() + 2);
}

#[test]
fn apply_subscripts_complex_sequence_matches_expected_result() {
	let layout = make_test_layout();
	let subscripts = vec![
		DynamicSubscript::Slice(odd()),
		new_axis(),
		ellipsis(),
		DynamicSubscript::Index(0),
		DynamicSubscript::Slice(even()),
		new_axis(),
		new_axis(),
		DynamicSubscript::Index(6),
	];

	let result = layout
		.apply_subscripts(&subscripts)
		.expect("subscripts must be valid");

	assert_eq!(result.extents(), &[60, 1, 56, 24, 5, 1, 1]);
	assert_eq!(result.strides(), &[1_720_320, 0, 7_680, 320, 32, 0, 0]);
	assert_eq!(result.offset(), 860_192);
}

#[test]
fn apply_subscripts_with_no_subscripts_on_default_layout_succeeds() {
	let layout = StridedLayout::default();
	let result = layout
		.apply_subscripts(&[])
		.expect("empty subscripts must be valid");

	assert_eq!(result, StridedLayout::default());
}

#[test]
fn apply_subscripts_with_ellipsis_on_default_layout_succeeds() {
	let layout = StridedLayout::default();
	let subscripts = vec![ellipsis()];
	let result = layout
		.apply_subscripts(&subscripts)
		.expect("ellipsis should be valid on empty layout");

	assert_eq!(result, StridedLayout::default());
}

#[test]
fn apply_subscripts_with_new_axis_on_default_layout_succeeds() {
	let layout = StridedLayout::default();
	let subscripts = vec![new_axis()];
	let result = layout
		.apply_subscripts(&subscripts)
		.expect("new_axis should be valid on empty layout");

	assert_eq!(result.extents(), &[1]);
	assert_eq!(result.strides(), &[0]);
	assert_eq!(result.offset(), 0);
}

#[test]
fn apply_subscripts_with_two_ellipsis_fails() {
	let layout = make_test_layout();
	let subscripts = vec![ellipsis(), new_axis(), ellipsis()];
	let error = layout.apply_subscripts(&subscripts).expect_err("must fail");

	assert_eq!(error, StridedLayoutError::MultipleEllipsis);
}

#[test]
fn apply_subscripts_with_too_many_subscripts_fails() {
	let layout = make_test_layout();
	let subscripts = vec![
		DynamicSubscript::Index(6),
		DynamicSubscript::Index(2),
		DynamicSubscript::Index(2),
		DynamicSubscript::Slice(odd()),
		DynamicSubscript::Slice(even()),
		DynamicSubscript::Slice(all()),
		DynamicSubscript::Index(2),
	];
	let error = layout.apply_subscripts(&subscripts).expect_err("must fail");

	assert_eq!(error, StridedLayoutError::NoMoreAxesForIndex);
}

#[test]
fn apply_subscripts_with_too_many_subscripts_and_ellipsis_fails() {
	let layout = make_test_layout();
	let subscripts = vec![
		DynamicSubscript::Index(6),
		DynamicSubscript::Index(2),
		DynamicSubscript::Index(2),
		ellipsis(),
		DynamicSubscript::Slice(odd()),
		DynamicSubscript::Slice(even()),
		DynamicSubscript::Slice(all()),
		DynamicSubscript::Index(2),
	];
	let error = layout.apply_subscripts(&subscripts).expect_err("must fail");

	assert_eq!(error, StridedLayoutError::NoMoreAxesForSlice);
}

#[test]
fn apply_subscripts_with_out_of_bounds_index_fails() {
	let layout = make_test_layout();
	let subscripts = vec![DynamicSubscript::Index(120)];
	let error = layout.apply_subscripts(&subscripts).expect_err("must fail");
	assert_eq!(
		error,
		StridedLayoutError::IndexOutOfBounds {
			index: 120,
			extent: 120,
		}
	);
}

#[test]
fn apply_subscripts_with_out_of_bounds_index_after_ellipsis_fails() {
	let layout = make_test_layout();
	let subscripts = vec![ellipsis(), DynamicSubscript::Index(8)];
	let error = layout.apply_subscripts(&subscripts).expect_err("must fail");
	assert_eq!(
		error,
		StridedLayoutError::IndexOutOfBounds {
			index: 8,
			extent: 8,
		}
	);
}

#[test]
fn apply_subscripts_with_out_of_bounds_slice_fails() {
	let layout = make_test_layout();
	let subscripts = vec![DynamicSubscript::Slice(make_slice(121))];
	let error = layout.apply_subscripts(&subscripts).expect_err("must fail");
	assert_eq!(
		error,
		StridedLayoutError::InvalidSlice(SliceError::PositiveSliceOverflowsExtent {
			count: 121,
			start: 0,
			step: 1,
			extent: 120,
		})
	);
}

#[test]
fn apply_subscripts_with_out_of_bounds_slice_after_ellipsis_fails() {
	let layout = make_test_layout();
	let subscripts = vec![ellipsis(), DynamicSubscript::Slice(make_slice(9))];
	let error = layout.apply_subscripts(&subscripts).expect_err("must fail");
	assert_eq!(
		error,
		StridedLayoutError::InvalidSlice(SliceError::PositiveSliceOverflowsExtent {
			count: 9,
			start: 0,
			step: 1,
			extent: 8,
		})
	);
}

#[test]
fn apply_subscripts_with_index_in_default_layout_fails() {
	let layout = StridedLayout::default();
	let subscripts = vec![DynamicSubscript::Index(0)];
	let error = layout.apply_subscripts(&subscripts).expect_err("must fail");

	assert_eq!(error, StridedLayoutError::NoMoreAxesForIndex);
}

#[test]
fn apply_subscripts_with_slice_in_default_layout_fails() {
	let layout = StridedLayout::default();
	let subscripts = vec![DynamicSubscript::Slice(make_slice(1))];
	let error = layout.apply_subscripts(&subscripts).expect_err("must fail");

	assert_eq!(error, StridedLayoutError::NoMoreAxesForSlice);
}
