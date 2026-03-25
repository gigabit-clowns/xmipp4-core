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
 * @brief Flags to control the behavior of the multi_array_access_layout build 
 * behavior.
 * 
 */
enum class multi_array_access_layout_build_flag_bits
{
	enable_reordering = binary::bit(0), ///< Reorder axes by memory locality.
	enable_coalescing = binary::bit(1), ///< Coalesce contiguous axes.
};

/**
 * @brief Set of multi_array_access_layout_build_flag_bits used to control the 
 * multi_array_access_layout build behavior.
 * 
 */
using multi_array_access_layout_build_flags 
	= binary::flagset<multi_array_access_layout_build_flag_bits>;

XMIPP4_CONSTEXPR 
const char* to_string(multi_array_access_layout_build_flag_bits x) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(
	std::basic_ostream<T>& os, 
	multi_array_access_layout_build_flag_bits x
);

} // namespace multidimensional
} // namespace xmipp4

#include "multi_array_access_layout_build_flags.inl"
