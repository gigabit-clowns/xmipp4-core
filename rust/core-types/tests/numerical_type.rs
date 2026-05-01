use xmipp4_core_types::{promote_types, NumericalType, NumericalTypeCategory};

#[test]
fn make_complex_behaviour_matches_expected_cases() {
	assert_eq!(
		NumericalType::Float32.make_complex(),
		NumericalType::ComplexFloat32
	);
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

#[test]
fn promote_types_matches_key_lattice_cases() {
	assert_eq!(
		promote_types(NumericalType::Int8, NumericalType::UInt8),
		NumericalType::Int16
	);
	assert_eq!(
		promote_types(NumericalType::UInt64, NumericalType::Int64),
		NumericalType::Int64
	);
	assert_eq!(
		promote_types(NumericalType::Float32, NumericalType::ComplexFloat16),
		NumericalType::ComplexFloat32
	);
	assert_eq!(
		promote_types(NumericalType::Boolean, NumericalType::Char8),
		NumericalType::Int16
	);
	assert_eq!(
		promote_types(NumericalType::Unknown, NumericalType::Float32),
		NumericalType::Unknown
	);
}

#[test]
fn promote_types_is_commutative() {
	let all_types = [
		NumericalType::Unknown,
		NumericalType::Boolean,
		NumericalType::Char8,
		NumericalType::Int8,
		NumericalType::UInt8,
		NumericalType::Int16,
		NumericalType::UInt16,
		NumericalType::Int32,
		NumericalType::UInt32,
		NumericalType::Int64,
		NumericalType::UInt64,
		NumericalType::Float16,
		NumericalType::Float32,
		NumericalType::Float64,
		NumericalType::ComplexFloat16,
		NumericalType::ComplexFloat32,
		NumericalType::ComplexFloat64,
	];

	for &a in &all_types {
		for &b in &all_types {
			assert_eq!(promote_types(a, b), promote_types(b, a));
		}
	}
}
