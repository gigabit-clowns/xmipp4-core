use xmipp4_core_types::StridedLayout;

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
