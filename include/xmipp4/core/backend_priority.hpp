#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file backend_priority.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides backend_priority enumeration and utility functions
 * @date 2023-11-20
 * 
 */

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
