// SPDX-License-Identifier: GPL-3.0-only

#include "array_access_layout_build_flags.hpp"

namespace xmipp4 
{
namespace multidimensional
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(array_access_layout_build_flag_bits x) noexcept
{
    switch (x)
    {
    case array_access_layout_build_flag_bits::enable_reordering:     
        return "enable_reordering";
    case array_access_layout_build_flag_bits::enable_coalescing:
        return "enable_coalescing";
    default: return "";
    }
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(
    std::basic_ostream<T>& os, 
    array_access_layout_build_flag_bits x
)
{
    return os << to_string(x);
}

} // namespace multidimensional
} // namespace xmipp4
