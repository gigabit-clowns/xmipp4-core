// SPDX-License-Identifier: GPL-3.0-only

#include "access_flags.hpp"

namespace xmipp4 
{

XMIPP4_INLINE_CONSTEXPR const char* 
to_string(access_flag_bits v) noexcept
{
    switch (v)
    {
    case access_flag_bits::read:    return "read";
    case access_flag_bits::write:   return "write";
    default: return "";
    }
}

template<typename T>
inline std::basic_ostream<T>& 
operator<<(std::basic_ostream<T>& os, access_flag_bits v)
{
    return os << to_string(v);
}

} // namespace xmipp4