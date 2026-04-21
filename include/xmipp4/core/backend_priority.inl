// SPDX-License-Identifier: GPL-3.0-only

#include "backend_priority.hpp"
#include "platform/enum_helpers.hpp"

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
	XMIPP4_ENUM_TO_STR_CASE(backend_priority, unsupported)
	XMIPP4_ENUM_TO_STR_CASE(backend_priority, fallback)
	XMIPP4_ENUM_TO_STR_CASE(backend_priority, normal)
	XMIPP4_ENUM_TO_STR_CASE(backend_priority, optimal)
	default: return "";
	}
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(
	std::basic_ostream<T>& os, 
	backend_priority priority
)
{
	return os << to_string(priority);
}

} // namespace xmipp4
