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

#include "../platform/constexpr.hpp"

#include <cstddef>
#include <string_view>

namespace xmipp4
{
namespace compute
{

enum class numerical_type
{
    unknown = -1,

    int8,
    uint8,
    int16,
    uint16,
    int32,
    uint32,
    int64,
    uint64,

    float16,
    brain_float16,
    float32,
    float64,

    complex_float16,
    complex_float32,
    complex_float64,

    // Add here

    count
};

XMIPP4_CONSTEXPR std::size_t get_size(numerical_type type) noexcept;
XMIPP4_CONSTEXPR bool is_unsigned(numerical_type type) noexcept;
XMIPP4_CONSTEXPR bool is_integer(numerical_type type) noexcept;
XMIPP4_CONSTEXPR bool is_float(numerical_type type) noexcept;
XMIPP4_CONSTEXPR bool is_complex(numerical_type type) noexcept;

XMIPP4_CONSTEXPR numerical_type make_complex(numerical_type type) noexcept;

XMIPP4_CONSTEXPR const char* to_string(numerical_type type) noexcept;
bool from_string(std::string_view str, numerical_type& type) noexcept;

} // namespace compute
} // namespace xmipp4

#include "numerical_type.inl"
