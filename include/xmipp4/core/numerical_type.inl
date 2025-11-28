// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "numerical_type.hpp"
#include "fixed_float.hpp"

#include "fixed_float.hpp"

#include <cstdint>
#include <complex>
#include <array>
#include <unordered_map>

namespace xmipp4
{

XMIPP4_INLINE_CONSTEXPR std::size_t get_size(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::int8: return sizeof(std::int8_t);
	case numerical_type::uint8: return sizeof(std::uint8_t);
	case numerical_type::int16: return sizeof(std::int16_t);
	case numerical_type::uint16: return sizeof(std::uint16_t);
	case numerical_type::int32: return sizeof(std::int32_t);
	case numerical_type::uint32: return sizeof(std::uint32_t);
	case numerical_type::int64: return sizeof(std::int64_t);
	case numerical_type::uint64: return sizeof(std::uint64_t);
	case numerical_type::float16: return sizeof(float16_t);
	case numerical_type::brain_float16: return 2; // Not available
	case numerical_type::float32: return sizeof(float32_t); 
	case numerical_type::float64: return  sizeof(float32_t);
	case numerical_type::complex_float16: return sizeof(std::complex<float16_t>);
	case numerical_type::complex_float32: return sizeof(std::complex<float32_t>);
	case numerical_type::complex_float64: return sizeof(std::complex<float32_t>);
	default: return 0;
	}
}

XMIPP4_INLINE_CONSTEXPR bool is_unsigned(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::uint8:
	case numerical_type::uint16:
	case numerical_type::uint32:
	case numerical_type::uint64:
		return true;
	default:
		return false;
	} 
}

XMIPP4_INLINE_CONSTEXPR bool is_integer(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::int8:
	case numerical_type::uint8:
	case numerical_type::int16:
	case numerical_type::uint16:
	case numerical_type::int32:
	case numerical_type::uint32:
	case numerical_type::int64:
	case numerical_type::uint64:
		return true;
	default:
		return false;
	} 
}

XMIPP4_INLINE_CONSTEXPR bool is_float(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::float16:
	case numerical_type::brain_float16:
	case numerical_type::float32:
	case numerical_type::float64:
		return true;
	default:
		return false;
	} 
}

XMIPP4_INLINE_CONSTEXPR bool is_complex(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::complex_float16:
	case numerical_type::complex_float32:
	case numerical_type::complex_float64:
		return true;
	default:
		return false;
	} 
}

XMIPP4_INLINE_CONSTEXPR 
numerical_type make_complex(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::float16: return numerical_type::complex_float16;
	case numerical_type::float32: return numerical_type::complex_float32;
	case numerical_type::float64: return numerical_type::complex_float64;
	case numerical_type::complex_float16: return numerical_type::complex_float16;
	case numerical_type::complex_float32: return numerical_type::complex_float32;
	case numerical_type::complex_float64: return numerical_type::complex_float64;
	default: return numerical_type::unknown;
	} 
}

XMIPP4_INLINE_CONSTEXPR 
numerical_type common_type(numerical_type x, numerical_type y) noexcept
{
	if (x == numerical_type::unknown || y == numerical_type::unknown)
	{
		return numerical_type::unknown;
	}

	XMIPP4_CONST_CONSTEXPR std::size_t N = 
		static_cast<std::size_t>(numerical_type::count);

	XMIPP4_CONST_CONSTEXPR std::array<std::array<numerical_type, N>, N> LUT = {
		std::array<numerical_type, N>{ // numerical_type::int8
			numerical_type::int8,
			numerical_type::int8,
			numerical_type::int16,
			numerical_type::int16,
			numerical_type::int32,
			numerical_type::int32,
			numerical_type::int64,
			numerical_type::int64,
			numerical_type::float16,
			numerical_type::brain_float16,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::uint8
			numerical_type::int8,
			numerical_type::uint8,
			numerical_type::int16,
			numerical_type::uint16,
			numerical_type::int32,
			numerical_type::uint32,
			numerical_type::int64,
			numerical_type::uint64,
			numerical_type::float16,
			numerical_type::brain_float16,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::int16
			numerical_type::int16,
			numerical_type::int16,
			numerical_type::int16,
			numerical_type::int16,
			numerical_type::int32,
			numerical_type::int32,
			numerical_type::int64,
			numerical_type::int64,
			numerical_type::float16,
			numerical_type::brain_float16,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::uint16
			numerical_type::int16,
			numerical_type::uint16,
			numerical_type::int16,
			numerical_type::uint16,
			numerical_type::int32,
			numerical_type::uint32,
			numerical_type::int64,
			numerical_type::uint64,
			numerical_type::float16,
			numerical_type::brain_float16,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::int32
			numerical_type::int32,
			numerical_type::int32,
			numerical_type::int32,
			numerical_type::int32,
			numerical_type::int32,
			numerical_type::int32,
			numerical_type::int64,
			numerical_type::int64,
			numerical_type::float16,
			numerical_type::brain_float16,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::uint32
			numerical_type::int32,
			numerical_type::uint32,
			numerical_type::int32,
			numerical_type::uint32,
			numerical_type::int32,
			numerical_type::uint32,
			numerical_type::int64,
			numerical_type::uint64,
			numerical_type::float16,
			numerical_type::brain_float16,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::int64
			numerical_type::int64,
			numerical_type::int64,
			numerical_type::int64,
			numerical_type::int64,
			numerical_type::int64,
			numerical_type::int64,
			numerical_type::int64,
			numerical_type::int64,
			numerical_type::float16,
			numerical_type::brain_float16,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::uint64 
			numerical_type::int64,
			numerical_type::uint64,
			numerical_type::int64,
			numerical_type::uint64,
			numerical_type::int64,
			numerical_type::uint64,
			numerical_type::int64,
			numerical_type::uint64,
			numerical_type::float16,
			numerical_type::brain_float16,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::float16
			numerical_type::float16,
			numerical_type::float16,
			numerical_type::float16,
			numerical_type::float16,
			numerical_type::float16,
			numerical_type::float16,
			numerical_type::float16,
			numerical_type::float16,
			numerical_type::float16,
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::bfloat16
			numerical_type::brain_float16,
			numerical_type::brain_float16,
			numerical_type::brain_float16,
			numerical_type::brain_float16,
			numerical_type::brain_float16,
			numerical_type::brain_float16,
			numerical_type::brain_float16,
			numerical_type::brain_float16,
			numerical_type::float32,
			numerical_type::brain_float16,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::float32
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float32,
			numerical_type::float64,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::float64
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::complex_float16
			numerical_type::complex_float16,
			numerical_type::complex_float16,
			numerical_type::complex_float16,
			numerical_type::complex_float16,
			numerical_type::complex_float16,
			numerical_type::complex_float16,
			numerical_type::complex_float16,
			numerical_type::complex_float16,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
			numerical_type::complex_float16,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::complex_float32
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
			numerical_type::complex_float32,
			numerical_type::complex_float32,
			numerical_type::complex_float64,
		},
		std::array<numerical_type, N>{ // numerical_type::complex_float64
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
			numerical_type::complex_float64,
		}
	};

	const auto index1 = static_cast<std::size_t>(x);
	const auto index2 = static_cast<std::size_t>(y);
	if (index1 < N && index2 < N)
	{
		return LUT[index1][index2];
	}
	else
	{
		return numerical_type::unknown;
	}
}


XMIPP4_INLINE_CONSTEXPR 
const char* to_string(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::unknown: return "unknown";
	case numerical_type::int8: return "int8";
	case numerical_type::uint8: return "uint8";
	case numerical_type::int16: return "int16";
	case numerical_type::uint16: return "uint16";
	case numerical_type::int32: return "int32";
	case numerical_type::uint32: return "uint32";
	case numerical_type::int64: return "int64";
	case numerical_type::uint64: return "uint64";
	case numerical_type::float16: return "float16";
	case numerical_type::brain_float16: return "brain_float16";
	case numerical_type::float32: return "float32";
	case numerical_type::float64: return "float64";
	case numerical_type::complex_float16: return "complex_float16";
	case numerical_type::complex_float32: return "complex_float32";
	case numerical_type::complex_float64: return "complex_float64";
	default: return "";
	}
}

template<typename T>
inline
std::basic_ostream<T>& 
operator<<(std::basic_ostream<T>& os, numerical_type type)
{
	return os << to_string(type);
}

} // namespace xmipp4
