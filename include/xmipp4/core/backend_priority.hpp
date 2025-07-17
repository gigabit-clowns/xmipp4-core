// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "platform/constexpr.hpp"
#include "platform/attributes.hpp"

#include <string_view>
#include <ostream>

namespace xmipp4 
{

/**
 * @brief Enumeration defining the preference over the selection of a
 * particular backend.
 * 
 */
enum class backend_priority
{
    fallback = -1024, ///< Used for dummy implementations.
    normal = 0, ///< Normal priority. Use by default.
    preferred = 1024, ///< Used when the backend should be preferred. Use with caution.
};



XMIPP4_CONSTEXPR 
bool operator<(backend_priority lhs, backend_priority rhs) noexcept;
XMIPP4_CONSTEXPR 
bool operator<=(backend_priority lhs, backend_priority rhs) noexcept;
XMIPP4_CONSTEXPR 
bool operator>(backend_priority lhs, backend_priority rhs) noexcept;
XMIPP4_CONSTEXPR 
bool operator>=(backend_priority lhs, backend_priority rhs) noexcept;



XMIPP4_CONSTEXPR const char* to_string(backend_priority priority) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, 
                                  backend_priority priority );

} // namespace xmipp4

#include "backend_priority.inl"
