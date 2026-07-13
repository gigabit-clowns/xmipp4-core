// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "numerical_type_dispatch.hpp"

#include "fixed_width_float.hpp"

#include <complex>
#include <stdexcept>

namespace xmipp4
{

namespace detail
{

template <typename F, typename... Entries>
XMIPP4_INLINE_CONSTEXPR
auto dispatch_single_numerical_type(
	F&& visitor, 
	type_map<Entries...> /*dispatch_map*/, 
	numerical_type type
) 
{
	using map = type_map<Entries...>;

    switch (type) 
	{
	case numerical_type::boolean: 
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::boolean, map>::type>()
		);
	case numerical_type::char8: 
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::char8, map>::type>()
		);
	case numerical_type::int8: 
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::int8, map>::type>()
		);
	case numerical_type::uint8:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::uint8, map>::type>()
		);
	case numerical_type::int16:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::int16, map>::type>()
		);
	case numerical_type::uint16:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::uint16, map>::type>()
		);
	case numerical_type::int32:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::int32, map>::type>()
		);
	case numerical_type::uint32:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::uint32, map>::type>()
		);
	case numerical_type::int64:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::int64, map>::type>()
		);
	case numerical_type::uint64:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::uint64, map>::type>()
		);
	case numerical_type::float16:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::float16, map>::type>()
		);
	case numerical_type::float32:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::float32, map>::type>()
		);
	case numerical_type::float64: 
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::float64, map>::type>()
		);
	case numerical_type::complex_float16:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::complex_float16, map>::type>()
		);
	case numerical_type::complex_float32:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::complex_float32, map>::type>()
		);
	case numerical_type::complex_float64:
		return std::forward<F>(visitor)(
			type_tag<typename type_map_element<numerical_type::complex_float64, map>::type>()
		);
	default: 
		throw std::invalid_argument("Unknown numerical type");
    }
}

template <typename F, typename... Entries>
XMIPP4_INLINE_CONSTEXPR
auto dispatch_numerical_types(F&& visitor, type_map<Entries...>) 
{
    return std::forward<F>(visitor)();
}

template <typename F, typename... Entries, typename... Types>
XMIPP4_INLINE_CONSTEXPR
auto dispatch_numerical_types(
	F&& visitor, 
	type_map<Entries...> dispatch_map, 
	numerical_type type, 
	Types ...other_types
) 
{
    return dispatch_single_numerical_type(
		[&visitor, dispatch_map, &other_types...](auto tag) 
		{
			const auto bound_visitor = [&](auto... other_tags) 
			{
   				return std::forward<F>(visitor)(tag, other_tags...);
        	};

        	return dispatch_numerical_types(
				bound_visitor,
				dispatch_map,
				other_types...
			);
   		},
		dispatch_map,
		type
	);
}

} // namespace detail

template <typename F, typename... Entries, typename... Types>
XMIPP4_INLINE_CONSTEXPR
auto dispatch_numerical_types(
	F&& visitor, 
	type_map<Entries...> dispatch_map, 
	Types&& ...types
)
{
	return detail::dispatch_numerical_types(
		std::forward<F>(visitor), 
		dispatch_map,
		types...
	);
}

template <typename F, typename... Types>
XMIPP4_INLINE_CONSTEXPR
auto dispatch_numerical_types(F&& visitor, Types&& ...types)
{
	return dispatch_numerical_types(
		std::forward<F>(visitor),
		native_type_map(),
		types...
	);
}

} // namespace xmipp4
