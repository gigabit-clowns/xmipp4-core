// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../meta/type_tag.hpp"
#include "numerical_type.hpp"
#include "fixed_float.hpp"
#include "../platform/constexpr.hpp"

#include <complex>
#include <type_traits>

namespace xmipp4
{

/**
 * @brief Entry in a `type_map`
 * 
 * @tparam Key Numerical to be used as a key in queries.
 * @tparam T Type associated to the the numerical type.
 */
template <numerical_type Key, typename T>
struct type_map_entry
{
	using type = T;
};

/**
 * @brief Static map relating types to `numerical_type`-s
 * 
 * Items may be accessed uing `type_map_element`.
 * 
 * @tparam Entries Entries stored in the map. Must be a specialization of 
 * `type_map_entry`
 * @see type_map_element
 */
template <typename... Entries>
struct type_map : Entries... {};

/**
 * @brief Query an entry of a type_map.
 * 
 * The queried type can be retrieved as the `type` member alias. If the key
 * can not be found on the map, the alias is set to `void`.
 * 
 * @tparam Key The key of the entry to be queried.
 * @tparam Map The `type_map` where the key is queried.
 */
template <numerical_type Key, typename Map>
struct type_map_element;

template <numerical_type Key>
struct type_map_element<Key, type_map<>> 
{
    using type = void; 
};

template <numerical_type Key, typename T, typename... Rest>
struct type_map_element<Key, type_map<type_map_entry<Key, T>, Rest...>> 
{
    using type = T;
};

template <numerical_type Key, numerical_type HeadKey, typename T, typename... Others>
struct type_map_element<Key, type_map<type_map_entry<HeadKey, T>, Others...>> 
{
    using type = typename type_map_element<Key, type_map<Others...>>::type;
};

/**
 * @brief Merge multiple `type_map`-s.
 * 
 * The merged type_map can be retrieved as the `type` member alias.
 * 
 * @tparam Maps Type maps to be merged. Must be specializations of `type_map`-s.
 */
template <typename... Maps>
struct type_map_cat;

template <>
struct type_map_cat<>
{
	using type = type_map<>;
};

template <typename... Entries>
struct type_map_cat<type_map<Entries...>>
{
	using type = type_map<Entries...>;
};

template <typename... Entries1, typename... Entries2, typename... Others>
struct type_map_cat<type_map<Entries1...>, type_map<Entries2...>, Others...>
{
	using type = typename type_map_cat<
		type_map<Entries1..., Entries2...>, 
		Others...
	>::type;
};


using native_boolean_entry = 
	type_map_entry<numerical_type::boolean, bool>;
using native_char8_entry = 
	type_map_entry<numerical_type::char8, char>;
using native_uint8_entry = 
	type_map_entry<numerical_type::uint8, std::uint8_t>;
using native_uint16_entry = 
	type_map_entry<numerical_type::uint16, std::uint16_t>;
using native_uint32_entry = 
	type_map_entry<numerical_type::uint32, std::uint32_t>;
using native_uint64_entry = 
	type_map_entry<numerical_type::uint64, std::uint64_t>;
using native_int8_entry = 
	type_map_entry<numerical_type::int8, std::int8_t>;
using native_int16_entry = 
	type_map_entry<numerical_type::int16, std::int16_t>;
using native_int32_entry = 
	type_map_entry<numerical_type::int32, std::int32_t>;
using native_int64_entry = 
	type_map_entry<numerical_type::int64, std::int64_t>;
using native_float16_entry = 
	type_map_entry<numerical_type::float16, xmipp4::float16_t>;
using native_float32_entry = 
	type_map_entry<numerical_type::float32, xmipp4::float32_t>;
using native_float64_entry = 
	type_map_entry<numerical_type::float64, xmipp4::float64_t>;
using native_complex_float16_entry = 
	type_map_entry<numerical_type::complex_float16, std::complex<xmipp4::float16_t>>;
using native_complex_float32_entry = 
	type_map_entry<numerical_type::complex_float32, std::complex<xmipp4::float32_t>>;
using native_complex_float64_entry = 
	type_map_entry<numerical_type::complex_float64, std::complex<xmipp4::float64_t>>;


using native_unsigned_integer_type_map = type_map<
	native_uint8_entry,
	native_uint16_entry,
	native_uint32_entry,
	native_uint64_entry
>;

using native_signed_integer_type_map = type_map<
	native_int8_entry,
	native_int16_entry,
	native_int32_entry,
	native_int64_entry
>;

using native_floating_type_map = type_map<
	native_float16_entry,
	native_float32_entry,
	native_float64_entry
>;

using native_complex_type_map = type_map<
	native_complex_float16_entry,
	native_complex_float32_entry,
	native_complex_float64_entry
>;

using native_integer_type_map = type_map_cat<
	native_unsigned_integer_type_map,
	native_signed_integer_type_map
>::type;

using native_arithmetic_type_map = type_map_cat<
	native_integer_type_map,
	native_floating_type_map,
	native_complex_type_map
>::type;

using native_type_map = type_map_cat<
	native_arithmetic_type_map,
	type_map<native_boolean_entry, native_char8_entry>
>::type;


/**
 * @brief Dispatch a function with the numerical types.
 * 
 * This function dispatches a set of numerical_type-s and calls the provided
 * visitor with the dispatched types wrapped around a type_tag. In case a type
 * fails conversion to a C++ type, type_tag<void> is passed.
 * 
 * @tparam F Functor to be dispatched.
 * @tparam Entries Specializations of type_map_entry-s.
 * @tparam Types any amount of numeric_type-s
 * @param visitor Function to be called with the dispatched types.
 * @param dispatch_map Instance of type_map used to generate type tags for 
 * dispatch. Unresolved types are dispatched with void.
 * @param types Types to be dispatched.
 * @return auto The result of calling visitor(types...)
 * 
 * @see numerical_type
 * @see type_tag
 */
template <typename F, typename... Entries, typename... Types>
XMIPP4_CONSTEXPR
auto dispatch_numerical_types(
	F&& visitor, 
	type_map<Entries...> dispatch_map, 
	Types&& ...types
);

/**
 * @brief Dispatch a function with the numerical types.
 * 
 * This function dispatches a set of numerical_type-s and calls the provided
 * visitor with the dispatched types wrapped around a type_tag. In case a type
 * fails conversion to a C++ type, type_tag<void> is passed.
 * 
 * @tparam F Functor to be dispatched.
 * @tparam Types any amount of numeric_type-s
 * @param visitor Function to be called with the dispatched types.
 * @param types Types to be dispatched.
 * @return auto The result of calling visitor(types...)
 * 
 * @see numerical_type
 * @see type_tag
 */
template <typename F, typename... Types>
XMIPP4_CONSTEXPR
auto dispatch_numerical_types(F&& visitor, Types&& ...types);


} // namespace xmipp4

#include "numerical_type_dispatch.inl"
