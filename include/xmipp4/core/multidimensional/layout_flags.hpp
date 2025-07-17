// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../binary/bit.hpp"
#include "../binary/flagset.hpp"
#include "../platform/constexpr.hpp"

#include <ostream>

namespace xmipp4 
{
namespace multidimensional
{

enum class layout_flag_bits
{
    contiguous = binary::bit(0),
    column_major = binary::bit(1),
    row_major = binary::bit(2)
};

using layout_flags = binary::flagset<layout_flag_bits>;



struct column_major_tag {};

/**
 * @brief Construct a column major tag
 * 
 * @return column_major_tag
 */
XMIPP4_CONSTEXPR column_major_tag column_major() noexcept;


struct row_major_tag {};

/**
 * @brief Construct a row major tag
 * 
 * @return row_major_tag
 */
XMIPP4_CONSTEXPR row_major_tag row_major() noexcept;

} // namespace multidimensional
} // namespace xmipp4

#include "layout_flags.inl"
