// SPDX-License-Identifier: GPL-3.0-only

#include "backend_priority.hpp"

namespace xmipp4 
{

XMIPP4_INLINE_CONSTEXPR 
bool operator<(backend_priority lhs, backend_priority rhs) noexcept
{
    return static_cast<std::underlying_type<backend_priority>::type>(lhs) <
           static_cast<std::underlying_type<backend_priority>::type>(rhs) ;  
}

XMIPP4_INLINE_CONSTEXPR 
bool operator<=(backend_priority lhs, backend_priority rhs) noexcept
{
    return static_cast<std::underlying_type<backend_priority>::type>(lhs) <=
           static_cast<std::underlying_type<backend_priority>::type>(rhs) ;  
}

XMIPP4_INLINE_CONSTEXPR 
bool operator>(backend_priority lhs, backend_priority rhs) noexcept
{
    return rhs < lhs;
}

XMIPP4_INLINE_CONSTEXPR 
bool operator>=(backend_priority lhs, backend_priority rhs) noexcept
{
    return rhs <= lhs;
}



XMIPP4_INLINE_CONSTEXPR 
const char* to_string(backend_priority priority) noexcept
{
    switch (priority)
    {
    case backend_priority::unsupported: return "unsupported";
    case backend_priority::fallback: return "fallback";
    case backend_priority::normal: return "normal";
    case backend_priority::optimal: return "optimal";
    default: return "";
    }
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, 
                                  backend_priority priority )
{
    return os << to_string(priority);
}

} // namespace xmipp4
