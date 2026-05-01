// SPDX-License-Identifier: GPL-3.0-only

use xmipp4_core_types::{
	compute_storage_requirement, is_initialized, ArrayDescriptor, NumericalType, StridedLayout,
};

#[test]
fn default_descriptor_is_not_initialized() {
	let descriptor = ArrayDescriptor::default();
	assert!(!is_initialized(&descriptor));
	assert_eq!(compute_storage_requirement(&descriptor), 0);
}

#[test]
fn initialized_depends_only_on_data_type() {
	let descriptor = ArrayDescriptor::new(StridedLayout::default(), NumericalType::Float32);
	assert!(is_initialized(&descriptor));
}

#[test]
fn descriptor_storage_requirement_uses_layout_and_data_type() {
	let layout = StridedLayout::make_contiguous_layout(vec![3, 4]);
	let descriptor = ArrayDescriptor::new(layout, NumericalType::Float32);
	assert!(is_initialized(&descriptor));
	assert_eq!(compute_storage_requirement(&descriptor), 3 * 4 * 4);
}

#[test]
fn unknown_data_type_has_zero_storage_requirement() {
	let layout = StridedLayout::make_contiguous_layout(vec![3, 4]);
	let descriptor = ArrayDescriptor::new(layout, NumericalType::Unknown);
	assert_eq!(compute_storage_requirement(&descriptor), 0);
}

#[test]
fn descriptor_equality_depends_on_type_and_layout() {
	let layout = StridedLayout::make_contiguous_layout(vec![2, 2]);
	let a = ArrayDescriptor::new(layout.clone(), NumericalType::Int16);
	let b = ArrayDescriptor::new(layout.clone(), NumericalType::Int16);
	let c = ArrayDescriptor::new(layout, NumericalType::UInt16);

	assert_eq!(a, b);
	assert_ne!(a, c);
}
