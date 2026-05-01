use xmipp4_core_types::{NumericalType, NumericalTypeCategory};

#[test]
fn make_complex_behaviour_matches_expected_cases() {
	assert_eq!(NumericalType::Float32.make_complex(), NumericalType::ComplexFloat32);
	assert_eq!(
		NumericalType::ComplexFloat64.make_complex(),
		NumericalType::ComplexFloat64
	);
	assert_eq!(NumericalType::Int32.make_complex(), NumericalType::Unknown);
}

#[test]
fn size_and_category_are_consistent() {
	assert_eq!(NumericalType::Float16.size_bytes(), Some(2));
	assert_eq!(NumericalType::ComplexFloat32.size_bytes(), Some(8));
	assert_eq!(
		NumericalType::UInt64.category(),
		NumericalTypeCategory::UnsignedInteger
	);
	assert_eq!(NumericalType::Unknown.size_bytes(), None);
}
