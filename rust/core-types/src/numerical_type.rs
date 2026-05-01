// SPDX-License-Identifier: GPL-3.0-only

//! Numerical type model and promotion logic.

/// Enumerates supported scalar and complex numerical representations.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum NumericalType {
	Unknown,
	Boolean,
	Char8,
	Int8,
	UInt8,
	Int16,
	UInt16,
	Int32,
	UInt32,
	Int64,
	UInt64,
	Float16,
	Float32,
	Float64,
	ComplexFloat16,
	ComplexFloat32,
	ComplexFloat64,
}

/// High-level category for a [`NumericalType`].
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum NumericalTypeCategory {
	Unknown,
	Boolean,
	Character,
	SignedInteger,
	UnsignedInteger,
	FloatingPoint,
	Complex,
}

impl NumericalType {
	/// Returns the size in bytes for values of this type.
	///
	/// Returns `None` for [`NumericalType::Unknown`].
	pub fn size_bytes(self) -> Option<usize> {
		match self {
			NumericalType::Unknown => None,
			NumericalType::Boolean => Some(1),
			NumericalType::Char8 => Some(1),
			NumericalType::Int8 | NumericalType::UInt8 => Some(1),
			NumericalType::Int16 | NumericalType::UInt16 | NumericalType::Float16 => Some(2),
			NumericalType::Int32 | NumericalType::UInt32 | NumericalType::Float32 => Some(4),
			NumericalType::Int64 | NumericalType::UInt64 | NumericalType::Float64 => Some(8),
			NumericalType::ComplexFloat16 => Some(4),
			NumericalType::ComplexFloat32 => Some(8),
			NumericalType::ComplexFloat64 => Some(16),
		}
	}

	/// Returns the logical category of this numerical type.
	pub fn category(self) -> NumericalTypeCategory {
		match self {
			NumericalType::Unknown => NumericalTypeCategory::Unknown,
			NumericalType::Boolean => NumericalTypeCategory::Boolean,
			NumericalType::Char8 => NumericalTypeCategory::Character,
			NumericalType::Int8
			| NumericalType::Int16
			| NumericalType::Int32
			| NumericalType::Int64 => NumericalTypeCategory::SignedInteger,
			NumericalType::UInt8
			| NumericalType::UInt16
			| NumericalType::UInt32
			| NumericalType::UInt64 => NumericalTypeCategory::UnsignedInteger,
			NumericalType::Float16 | NumericalType::Float32 | NumericalType::Float64 => {
				NumericalTypeCategory::FloatingPoint
			}
			NumericalType::ComplexFloat16
			| NumericalType::ComplexFloat32
			| NumericalType::ComplexFloat64 => NumericalTypeCategory::Complex,
		}
	}

	/// Returns the matching complex type for floating-point inputs.
	///
	/// Returns [`NumericalType::Unknown`] for unsupported categories.
	pub fn make_complex(self) -> NumericalType {
		match self {
			NumericalType::Float16 | NumericalType::ComplexFloat16 => NumericalType::ComplexFloat16,
			NumericalType::Float32 | NumericalType::ComplexFloat32 => NumericalType::ComplexFloat32,
			NumericalType::Float64 | NumericalType::ComplexFloat64 => NumericalType::ComplexFloat64,
			_ => NumericalType::Unknown,
		}
	}
}

/// Promotes two types using the parity lattice equivalent to the C++ model.
///
/// Returns [`NumericalType::Unknown`] when no unique least common promotable
/// type exists.
pub fn promote_types(type1: NumericalType, type2: NumericalType) -> NumericalType {
	if matches!(type1, NumericalType::Unknown) || matches!(type2, NumericalType::Unknown) {
		return NumericalType::Unknown;
	}

	if type1 == type2 {
		return type1;
	}

	let common = collect_common_promotable_types(type1, type2);

	if common.is_empty() {
		return NumericalType::Unknown;
	}

	let minimal = collect_minimal_common_types(&common);

	if minimal.len() == 1 {
		minimal[0]
	} else {
		NumericalType::Unknown
	}
}

fn collect_common_promotable_types(
	type1: NumericalType,
	type2: NumericalType,
) -> Vec<NumericalType> {
	let promotable_from_type1 = collect_reachable_promotions(type1);
	let promotable_from_type2 = collect_reachable_promotions(type2);

	promotable_from_type1
		.into_iter()
		.filter(|candidate| promotable_from_type2.contains(candidate))
		.collect::<Vec<_>>()
}

fn collect_minimal_common_types(common_types: &[NumericalType]) -> Vec<NumericalType> {
	common_types
		.iter()
		.copied()
		.filter(|candidate| {
			!common_types
				.iter()
				.copied()
				.any(|other| other != *candidate && has_promotion_path(other, *candidate))
		})
		.collect::<Vec<_>>()
}

fn collect_reachable_promotions(from: NumericalType) -> Vec<NumericalType> {
	let mut stack = vec![from];
	let mut visited = Vec::new();

	while let Some(current) = stack.pop() {
		if mark_visited_once(&mut visited, current) {
			continue;
		}

		for &successor in promotion_successors(current) {
			if !visited.contains(&successor) {
				stack.push(successor);
			}
		}
	}

	visited
}

fn has_promotion_path(from: NumericalType, to: NumericalType) -> bool {
	if from == to {
		return true;
	}

	let mut stack = vec![from];
	let mut visited = Vec::new();

	while let Some(current) = stack.pop() {
		if current == to {
			return true;
		}

		if mark_visited_once(&mut visited, current) {
			continue;
		}

		for &successor in promotion_successors(current) {
			if !visited.contains(&successor) {
				stack.push(successor);
			}
		}
	}

	false
}

fn mark_visited_once(visited: &mut Vec<NumericalType>, current: NumericalType) -> bool {
	if visited.contains(&current) {
		return true;
	}

	visited.push(current);
	false
}

fn promotion_successors(value: NumericalType) -> &'static [NumericalType] {
	match value {
		NumericalType::Unknown => &[],
		NumericalType::Boolean => &[NumericalType::UInt8, NumericalType::Int8],
		NumericalType::Char8 => &[NumericalType::Int16],
		NumericalType::Int8 => &[NumericalType::Int16],
		NumericalType::UInt8 => &[NumericalType::UInt16, NumericalType::Int16],
		NumericalType::Int16 => &[NumericalType::Int32],
		NumericalType::UInt16 => &[NumericalType::UInt32, NumericalType::Int32],
		NumericalType::Int32 => &[NumericalType::Int64],
		NumericalType::UInt32 => &[NumericalType::UInt64, NumericalType::Int64],
		NumericalType::Int64 => &[NumericalType::Float16],
		NumericalType::UInt64 => &[NumericalType::Int64],
		NumericalType::Float16 => &[NumericalType::Float32, NumericalType::ComplexFloat16],
		NumericalType::Float32 => &[NumericalType::Float64, NumericalType::ComplexFloat32],
		NumericalType::Float64 => &[NumericalType::ComplexFloat64],
		NumericalType::ComplexFloat16 => &[NumericalType::ComplexFloat32],
		NumericalType::ComplexFloat32 => &[NumericalType::ComplexFloat64],
		NumericalType::ComplexFloat64 => &[],
	}
}
