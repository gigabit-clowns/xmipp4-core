// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../binary/bit.hpp"
#include "../binary/flagset.hpp"

#include <ostream>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Flags to control the behavior of the array_access_layout build 
 * behavior.
 * 
 */
enum class array_access_layout_build_flag_bits
{
	enable_reordering = binary::bit(0), ///< Reorder axes by memory locality.
	enable_coalescing = binary::bit(1), ///< Coalesce contiguous axes.
};

/**
 * @brief Set of array_access_layout_build_flag_bits used to control the 
 * array_access_layout build behavior.
 * 
 */
using array_access_layout_build_flags 
	= binary::flagset<array_access_layout_build_flag_bits>;

XMIPP4_CONSTEXPR 
const char* to_string(array_access_layout_build_flag_bits x) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(
	std::basic_ostream<T>& os, 
	array_access_layout_build_flag_bits x
);

} // namespace multidimensional
} // namespace xmipp4

#include "array_access_layout_build_flags.inl"
