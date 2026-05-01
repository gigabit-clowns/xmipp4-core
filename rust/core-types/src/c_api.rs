// SPDX-License-Identifier: GPL-3.0-only

//! C ABI bridge used by optional C++ integration.

use crate::{promote_types, NumericalType, StridedLayout};

#[no_mangle]
pub extern "C" fn xmipp4_rust_promote_types(type1: i32, type2: i32) -> i32 {
	let type1 = from_c_numerical_type(type1);
	let type2 = from_c_numerical_type(type2);
	to_c_numerical_type(promote_types(type1, type2))
}

#[no_mangle]
pub extern "C" fn xmipp4_rust_compute_storage_requirement(
	extents: *const usize,
	strides: *const isize,
	rank: usize,
	offset: isize,
) -> usize {
	if rank == 0 {
		return StridedLayout::default().compute_storage_requirement();
	}

	if extents.is_null() || strides.is_null() {
		return 0;
	}

	let extents = unsafe { std::slice::from_raw_parts(extents, rank) }.to_vec();
	let strides = unsafe { std::slice::from_raw_parts(strides, rank) }.to_vec();

	StridedLayout::new(extents, strides, offset)
		.map(|layout| layout.compute_storage_requirement())
		.unwrap_or(0)
}

fn from_c_numerical_type(value: i32) -> NumericalType {
	match value {
		-1 => NumericalType::Unknown,
		0 => NumericalType::Boolean,
		1 => NumericalType::Char8,
		2 => NumericalType::Int8,
		3 => NumericalType::UInt8,
		4 => NumericalType::Int16,
		5 => NumericalType::UInt16,
		6 => NumericalType::Int32,
		7 => NumericalType::UInt32,
		8 => NumericalType::Int64,
		9 => NumericalType::UInt64,
		10 => NumericalType::Float16,
		11 => NumericalType::Float32,
		12 => NumericalType::Float64,
		13 => NumericalType::ComplexFloat16,
		14 => NumericalType::ComplexFloat32,
		15 => NumericalType::ComplexFloat64,
		_ => NumericalType::Unknown,
	}
}

fn to_c_numerical_type(value: NumericalType) -> i32 {
	match value {
		NumericalType::Unknown => -1,
		NumericalType::Boolean => 0,
		NumericalType::Char8 => 1,
		NumericalType::Int8 => 2,
		NumericalType::UInt8 => 3,
		NumericalType::Int16 => 4,
		NumericalType::UInt16 => 5,
		NumericalType::Int32 => 6,
		NumericalType::UInt32 => 7,
		NumericalType::Int64 => 8,
		NumericalType::UInt64 => 9,
		NumericalType::Float16 => 10,
		NumericalType::Float32 => 11,
		NumericalType::Float64 => 12,
		NumericalType::ComplexFloat16 => 13,
		NumericalType::ComplexFloat32 => 14,
		NumericalType::ComplexFloat64 => 15,
	}
}
