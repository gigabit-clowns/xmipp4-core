// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "numerical_type.hpp"

#include "fixed_float.hpp"

#include <cstdint>
#include <complex>
#include <type_traits>

namespace xmipp4
{

template <>
struct numerical_type_of<char>
	: std::integral_constant<numerical_type, numerical_type::char8>
{
};

template <>
struct numerical_type_of<std::int8_t>
	: std::integral_constant<numerical_type, numerical_type::int8>
{
};

template <>
struct numerical_type_of<std::uint8_t>
	: std::integral_constant<numerical_type, numerical_type::uint8>
{
};

template <>
struct numerical_type_of<std::int16_t>
	: std::integral_constant<numerical_type, numerical_type::int16>
{
};

template <>
struct numerical_type_of<std::uint16_t>
	: std::integral_constant<numerical_type, numerical_type::uint16>
{
};

template <>
struct numerical_type_of<std::int32_t>
	: std::integral_constant<numerical_type, numerical_type::int32>
{
};

template <>
struct numerical_type_of<std::uint32_t>
	: std::integral_constant<numerical_type, numerical_type::uint32>
{
};

template <>
struct numerical_type_of<std::int64_t>
	: std::integral_constant<numerical_type, numerical_type::int64>
{
};

template <>
struct numerical_type_of<std::uint64_t>
	: std::integral_constant<numerical_type, numerical_type::uint64>
{
};

template <>
struct numerical_type_of<float16_t>
	: std::integral_constant<numerical_type, numerical_type::float16>
{
};

template <>
struct numerical_type_of<float32_t>
	: std::integral_constant<numerical_type, numerical_type::float32>
{
};

template <>
struct numerical_type_of<float64_t>
	: std::integral_constant<numerical_type, numerical_type::float64>
{
};

template <>
struct numerical_type_of<std::complex<float16_t>>
	: std::integral_constant<numerical_type, numerical_type::complex_float16>
{
};

template <>
struct numerical_type_of<std::complex<float32_t>>
	: std::integral_constant<numerical_type, numerical_type::complex_float32>
{
};

template <>
struct numerical_type_of<std::complex<float64_t>>
	: std::integral_constant<numerical_type, numerical_type::complex_float64>
{
};

namespace detail
{

template <typename F>
XMIPP4_INLINE_CONSTEXPR
auto visit_single(F&& visitor, numerical_type type) 
{
    switch (type) 
	{
	case numerical_type::char8: 
		return std::forward<F>(visitor)(type_tag<char>());
	case numerical_type::int8: 
		return std::forward<F>(visitor)(type_tag<std::int8_t>());
	case numerical_type::uint8:
		return std::forward<F>(visitor)(type_tag<std::uint8_t>());
	case numerical_type::int16:
		return std::forward<F>(visitor)(type_tag<std::int16_t>());
	case numerical_type::uint16:
		return std::forward<F>(visitor)(type_tag<std::uint16_t>());
	case numerical_type::int32:
		return std::forward<F>(visitor)(type_tag<std::int32_t>());
	case numerical_type::uint32:
		return std::forward<F>(visitor)(type_tag<std::uint32_t>());
	case numerical_type::int64:
		return std::forward<F>(visitor)(type_tag<std::int64_t>());
	case numerical_type::uint64:
		return std::forward<F>(visitor)(type_tag<std::uint64_t>());
	case numerical_type::float16:
		return std::forward<F>(visitor)(type_tag<float16_t>());
	case numerical_type::float32:
		return std::forward<F>(visitor)(type_tag<float32_t>());
	case numerical_type::float64: 
		return std::forward<F>(visitor)(type_tag<float64_t>());
	case numerical_type::complex_float16:
		return std::forward<F>(visitor)(type_tag<std::complex<float16_t>>());
	case numerical_type::complex_float32:
		return std::forward<F>(visitor)(type_tag<std::complex<float32_t>>());
	case numerical_type::complex_float64:
		return std::forward<F>(visitor)(type_tag<std::complex<float64_t>>());
	default: 
		return std::forward<F>(visitor)(type_tag<void>());
    }
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
auto visit(F&& visitor) 
{
    return std::forward<F>(visitor)();
}

template <typename F, typename... Types>
XMIPP4_INLINE_CONSTEXPR
auto visit(
	F&& visitor, 
	numerical_type type, 
	Types ...other_types
) 
{
    return visit_single(
		[&visitor, &other_types...](auto tag) 
		{
			const auto bound_visitor = [&](auto... other_tags) 
			{
   				return std::forward<F>(visitor)(tag, other_tags...);
        	};

        	return visit(
				bound_visitor,
				other_types...
			);
   		},
		type
	);
}

} // namespace detail

template <typename F, typename... Types>
XMIPP4_INLINE_CONSTEXPR
auto visit(F&& visitor, Types&& ...types)
{
	return detail::visit(std::forward<F>(visitor), types...);
}

namespace detail
{

template <typename T>
struct size_of
	: std::integral_constant<std::size_t, sizeof(T)>
{
};

template <>
struct size_of<void>
	: std::integral_constant<std::size_t, 0UL>
{
};

} // namespace detail

XMIPP4_INLINE_CONSTEXPR 
std::size_t get_size(numerical_type type) noexcept
{
	return visit(
		[] (auto tag)
		{
			using type = typename decltype(tag)::type;
			return detail::size_of<type>::value;
		},
		type
	);
}

XMIPP4_INLINE_CONSTEXPR 
bool is_unsigned(numerical_type type) noexcept
{
	return visit(
		[] (auto tag)
		{
			using type = typename decltype(tag)::type;
			return std::is_unsigned<type>::value;
		},
		type
	);
}

XMIPP4_INLINE_CONSTEXPR 
bool is_integer(numerical_type type) noexcept
{
	return visit(
		[] (auto tag)
		{
			using type = typename decltype(tag)::type;
			return std::is_integral<type>::value;
		},
		type
	);
}

namespace detail
{

template <typename T>
struct is_floating_point
	: std::is_floating_point<T>
{
};

template <>
struct is_floating_point<half>
	: std::true_type
{
};

} // namespace detail

XMIPP4_INLINE_CONSTEXPR 
bool is_float(numerical_type type) noexcept
{
	return visit(
		[] (auto tag)
		{
			using type = typename decltype(tag)::type;
			return detail::is_floating_point<type>::value;
		},
		type
	);
}

namespace detail
{

template <typename T>
struct is_complex
	: std::false_type
{
};

template <typename T>
struct is_complex<std::complex<T>>
	: std::true_type
{
};

} // namespace detail

XMIPP4_INLINE_CONSTEXPR 
bool is_complex(numerical_type type) noexcept
{
	return visit(
		[] (auto tag)
		{
			using type = typename decltype(tag)::type;
			return detail::is_complex<type>::value;
		},
		type
	);
}

XMIPP4_INLINE_CONSTEXPR 
numerical_type make_complex(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::float16: return numerical_type::complex_float16;
	case numerical_type::float32: return numerical_type::complex_float32;
	case numerical_type::float64: return numerical_type::complex_float64;
	case numerical_type::complex_float16: 
		return numerical_type::complex_float16;
	case numerical_type::complex_float32: 
		return numerical_type::complex_float32;
	case numerical_type::complex_float64: 
		return numerical_type::complex_float64;
	default: return numerical_type::unknown;
	} 
}

namespace detail
{

template <std::size_t Bytes, bool Signed> struct sized_integer;
template <> struct sized_integer<1, true>  { using type = std::int8_t; };
template <> struct sized_integer<1, false> { using type = std::uint8_t; };
template <> struct sized_integer<2, true>  { using type = std::int16_t; };
template <> struct sized_integer<2, false> { using type = std::uint16_t; };
template <> struct sized_integer<4, true>  { using type = std::int32_t; };
template <> struct sized_integer<4, false> { using type = std::uint32_t; };
template <> struct sized_integer<8, true>  { using type = std::int64_t; };
template <> struct sized_integer<8, false> { using type = std::uint64_t; };

template <typename T, typename Q>
struct common_integer_type {
    using type = 
		typename std::conditional<
			std::is_same<T, char>::value && std::is_same<Q, char>::value,
			char,
			typename sized_integer<
				std::max(sizeof(T), sizeof(Q)), 
				std::is_signed<T>::value || std::is_signed<Q>::value
			>::type
		>::type;
	};

template <typename T>
struct scalar_score : std::integral_constant<int, 0> {};
template <> 
struct scalar_score<half_float::half> : std::integral_constant<int, 1> {};
template <> 
struct scalar_score<float> : std::integral_constant<int, 2> {};
template <> 
struct scalar_score<double> : std::integral_constant<int, 3> {};

template <typename T, typename Q>
struct common_scalar_type
{
	using type = typename std::conditional<
		std::is_integral<T>::value && std::is_integral<Q>::value,
		typename common_integer_type<T, Q>::type,
		typename std::conditional<
			(scalar_score<T>::value > scalar_score<Q>::value), 
			T,
			Q
		>::type
	>::type;
};

template <typename T>
struct remove_complex 
{ 
	using type = T; 
};

template <typename T>
struct remove_complex<std::complex<T>> 
{ 
	using type = T; 
};

template <typename T, typename Q>
struct common_numerical_type
{
private:
	using scalar_type = typename common_scalar_type<
		typename remove_complex<T>::type,
		typename remove_complex<Q>::type
	>::type;
public:
	using type = typename std::conditional<
		is_complex<T>::value || is_complex<Q>::value,
		std::complex<scalar_type>,
		scalar_type 
	>::type;
};

template <typename T, typename Q>
struct common_type { using type = typename common_numerical_type<T, Q>::type; };
template <typename T> 
struct common_type<void, T> { using type = void; };
template <typename T>
struct common_type<T, void> { using type = void; };
template <>
struct common_type<void, void> { using type = void; };

template <typename T>
struct permissive_numerical_type_of
	: numerical_type_of<T>
{
};

template <>
struct permissive_numerical_type_of<void>
	: std::integral_constant<numerical_type, numerical_type::unknown>
{
};

} // namespace detail

XMIPP4_INLINE_CONSTEXPR 
numerical_type common_type(numerical_type type1, numerical_type type2) noexcept
{
	return visit(
		[] (auto tag1, auto tag2)
		{
			using type1 = typename decltype(tag1)::type;
			using type2 = typename decltype(tag2)::type;
			using common_type = 
				typename detail::common_type<type1, type2>::type;
			return detail::permissive_numerical_type_of<common_type>::value;
		},
		type1,
		type2
	);
}

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(numerical_type type) noexcept
{
	switch (type)
	{
	case numerical_type::unknown: return "unknown";
	case numerical_type::char8: return "char8";
	case numerical_type::int8: return "int8";
	case numerical_type::uint8: return "uint8";
	case numerical_type::int16: return "int16";
	case numerical_type::uint16: return "uint16";
	case numerical_type::int32: return "int32";
	case numerical_type::uint32: return "uint32";
	case numerical_type::int64: return "int64";
	case numerical_type::uint64: return "uint64";
	case numerical_type::float16: return "float16";
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
