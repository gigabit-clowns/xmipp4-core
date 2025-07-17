// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <type_traits>
#include <cstddef>

namespace xmipp4 
{

template <typename T, typename=void>
struct is_index
	: std::false_type
{
};

template <typename I>
struct is_index<I, typename std::enable_if<std::is_integral<I>::value>::type>
	: std::true_type
{
};

template <typename I, I value>
struct is_index<std::integral_constant<I, value>>
	: std::true_type
{
};

template <typename I>
std::size_t sanitize_index(I index, std::size_t length);

} // namespace xmipp4

#include "index.inl"
