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

	pub fn make_complex(self) -> NumericalType {
		match self {
			NumericalType::Float16 | NumericalType::ComplexFloat16 => NumericalType::ComplexFloat16,
			NumericalType::Float32 | NumericalType::ComplexFloat32 => NumericalType::ComplexFloat32,
			NumericalType::Float64 | NumericalType::ComplexFloat64 => NumericalType::ComplexFloat64,
			_ => NumericalType::Unknown,
		}
	}
}
