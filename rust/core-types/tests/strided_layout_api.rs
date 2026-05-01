use xmipp4_core_types::StridedLayout;

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
	assert_eq!(layout.compute_storage_requirement(), 0);
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
	assert_eq!(layout.compute_storage_requirement(), 6);
}

#[test]
fn constructor_rejects_mismatched_rank() {
	let result = StridedLayout::new(vec![2, 2], vec![1], 0);
	assert!(result.is_err());
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

	assert_eq!(error, "Index 5 is missing in the axis permutation");
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

	assert_eq!(
		error,
		"Axis permutation's length does not match the required count"
	);
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
	assert_eq!(error, "Index 6 is out of bounds for extent 6");

	let error = layout
		.matrix_transpose(0, 6)
		.expect_err("axis must be out of bounds");
	assert_eq!(error, "Index 6 is out of bounds for extent 6");
}

#[test]
fn matrix_transpose_on_default_layout_always_fails() {
	let layout = StridedLayout::default();

	let error = layout
		.matrix_transpose(0, 0)
		.expect_err("default layout must fail");
	assert_eq!(error, "Index 0 is out of bounds for extent 0");

	let error = layout
		.matrix_transpose(1, 1)
		.expect_err("default layout must fail");
	assert_eq!(error, "Index 1 is out of bounds for extent 0");
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
	assert_eq!(error, "Index 6 is out of bounds for extent 6");

	let error = layout
		.matrix_diagonal(0, 6)
		.expect_err("axis must be out of bounds");
	assert_eq!(error, "Index 6 is out of bounds for extent 6");
}

#[test]
fn matrix_diagonal_on_default_layout_always_fails() {
	let layout = StridedLayout::default();

	let error = layout
		.matrix_diagonal(0, 0)
		.expect_err("default layout must fail");
	assert_eq!(error, "Index 0 is out of bounds for extent 0");

	let error = layout
		.matrix_diagonal(1, 0)
		.expect_err("default layout must fail");
	assert_eq!(error, "Index 1 is out of bounds for extent 0");
}

#[test]
fn matrix_diagonal_requires_different_axes() {
	let layout = make_test_layout();
	let error = layout.matrix_diagonal(0, 0).expect_err("must fail");

	assert_eq!(error, "axis1 and axis2 must represent different axes");
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
		"Cannot broadcast layout with 6 axes into a shape of 5 dimensions."
	);
}

#[test]
fn broadcast_to_fails_when_axis_is_not_broadcastable() {
	let layout = make_test_layout();
	let target_extents = vec![120, 55, 24, 1, 10, 8];

	let error = layout.broadcast_to(&target_extents).expect_err("must fail");
	assert_eq!(
		error,
		"Cannot broadcast axis of extent 56 into an extent of 55."
	);
}
