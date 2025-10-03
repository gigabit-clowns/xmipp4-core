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
 * @brief Flags to control the behavior of the kernel_access_layout build 
 * behavior.
 * 
 */
enum class kernel_access_layout_build_flag_bits
{
    reorder_batches = binary::bit(0), ///< Reorder batch axes by memory locality.
    coalesce_batches = binary::bit(1), ///< Coalesce contiguous batch axes.
};

/**
 * @brief Set of kernel_access_layout_build_flag_bits used to control the 
 * kernel_access_layout build behavior.
 * 
 */
using kernel_access_layout_build_flags 
    = binary::flagset<kernel_access_layout_build_flag_bits>;

XMIPP4_CONSTEXPR 
const char* to_string(kernel_access_layout_build_flag_bits x) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(
    std::basic_ostream<T>& os, 
    kernel_access_layout_build_flag_bits x
);

} // namespace multidimensional
} // namespace xmipp4

#include "kernel_access_layout_build_flags.inl"
