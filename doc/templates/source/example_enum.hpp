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
 * @file example_enum.hpp
 * @author Your Name (yourname@yourdomain.tld)
 * @brief Provides example_enum
 * @date Today (YYYY/MM/DD)
 * 
 */

#include "platform/constexpr.hpp"

#include <string_view>
#include <ostream>

namespace xmipp4 
{

/**
 * @brief Describe the purpose of the example_enum
 * 
 */
enum class example_enum
{
    first,
    second,
    third,
};

XMIPP4_CONSTEXPR const char* to_string(example_enum ex) noexcept;
bool from_string(std::string_view str, example_enum& ex) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, example_enum ex);

} // namespace xmipp4

#include "example_enum.inl"
