// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file layout_flags.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of layout_flags.hpp
 * @date 2024-05-01
 * 
 */

#include "layout_flags.hpp"

#include <unordered_map>

namespace xmipp4 
{
namespace multidimensional
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(layout_flag_bits flag) noexcept
{
    switch (flag)
    {
    case layout_flag_bits::contiguous: return "contiguous";
    case layout_flag_bits::column_major: return "column_major";
    case layout_flag_bits::row_major: return "row_major";
    default: return "";
    }
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, layout_flag_bits flag)
{
    return os << to_string(flag);
}





XMIPP4_INLINE_CONSTEXPR column_major_tag column_major() noexcept
{
    return column_major_tag();
}

XMIPP4_INLINE_CONSTEXPR row_major_tag row_major() noexcept
{
    return row_major_tag();
}

} // namespace multidimensional
} // namespace xmipp4
