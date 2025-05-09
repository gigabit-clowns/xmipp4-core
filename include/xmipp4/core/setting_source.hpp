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
 * @file setting_source.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides setting_source enumeration and utility functions
 * @date 2025-05-09
 * 
 */

#include "platform/constexpr.hpp"
#include "platform/attributes.hpp"

#include <string_view>
#include <ostream>

namespace xmipp4 
{

/**
 * @brief Defines possible sources for a setting in a preference order.
 * 
 */
enum class setting_source
{
    unset, ///< The value has not been set yet.
    preset, ///< The value is the default value.
    environment_variable, ///< The value is set by an environment variable.
    user, ///< The value is set by the user.
};

XMIPP4_CONSTEXPR 
bool operator<(setting_source lhs, setting_source rhs) noexcept;
XMIPP4_CONSTEXPR 
bool operator<=(setting_source lhs, setting_source rhs) noexcept;
XMIPP4_CONSTEXPR 
bool operator>(setting_source lhs, setting_source rhs) noexcept;
XMIPP4_CONSTEXPR 
bool operator>=(setting_source lhs, setting_source rhs) noexcept;



XMIPP4_CONSTEXPR const char* to_string(setting_source priority) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, 
                                  setting_source priority );

} // namespace xmipp4

#include "setting_source.inl"
