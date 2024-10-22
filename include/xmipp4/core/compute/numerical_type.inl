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

#include "numerical_type.hpp"

#include <unordered_map>

namespace xmipp4
{
namespace compute
{

XMIPP4_INLINE_CONSTEXPR std::size_t get_size(numerical_type type) noexcept
{
    switch (type)
    {
    case numerical_type::int8: return 1;
    case numerical_type::uint8: return 1;
    case numerical_type::int16: return 2;
    case numerical_type::uint16: return 2;
    case numerical_type::int32: return 4;
    case numerical_type::uint32: return 4;
    case numerical_type::int64: return 8;
    case numerical_type::uint64: return 8;
    case numerical_type::float16: return 2;
    case numerical_type::brain_float16: return 2;
    case numerical_type::float32: return 4;
    case numerical_type::float64: return 8;
    case numerical_type::complex_float16: return 4;
    case numerical_type::complex_float32: return 8;
    case numerical_type::complex_float64: return 16;
    default: return 0;
    }
}

XMIPP4_INLINE_CONSTEXPR bool is_unsigned(numerical_type type) noexcept
{
    switch (type)
    {
    case numerical_type::uint8:
    case numerical_type::uint16:
    case numerical_type::uint32:
    case numerical_type::uint64:
        return true;
    default:
        return false;
    } 
}

XMIPP4_INLINE_CONSTEXPR bool is_integer(numerical_type type) noexcept
{
    switch (type)
    {
    case numerical_type::int8:
    case numerical_type::uint8:
    case numerical_type::int16:
    case numerical_type::uint16:
    case numerical_type::int32:
    case numerical_type::uint32:
    case numerical_type::int64:
    case numerical_type::uint64:
        return true;
    default:
        return false;
    } 
}

XMIPP4_INLINE_CONSTEXPR bool is_float(numerical_type type) noexcept
{
    switch (type)
    {
    case numerical_type::float16:
    case numerical_type::brain_float16:
    case numerical_type::float32:
    case numerical_type::float64:
        return true;
    default:
        return false;
    } 
}

XMIPP4_INLINE_CONSTEXPR bool is_complex(numerical_type type) noexcept
{
    switch (type)
    {
    case numerical_type::complex_float16:
    case numerical_type::complex_float32:
    case numerical_type::complex_float64:
        return true;
    default:
        return false;
    } 
}

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(numerical_type type) noexcept
{
    switch (type)
    {
    case numerical_type::unknown: return "unknown";
    case numerical_type::int8: return "int8";
    case numerical_type::uint8: return "uint8";
    case numerical_type::int16: return "int16";
    case numerical_type::uint16: return "uint16";
    case numerical_type::int32: return "int32";
    case numerical_type::uint32: return "uint32";
    case numerical_type::int64: return "int64";
    case numerical_type::uint64: return "uint64";
    case numerical_type::float16: return "float16";
    case numerical_type::brain_float16: return "brain_float16";
    case numerical_type::float32: return "float32";
    case numerical_type::float64: return "float64";
    case numerical_type::complex_float16: return "complex_float16";
    case numerical_type::complex_float32: return "complex_float32";
    case numerical_type::complex_float64: return "complex_float64";
    }
}

inline
bool from_string(std::string_view str, numerical_type& type) noexcept
{
    static const
    std::unordered_map<std::string_view, numerical_type> str_2_numerical_type = 
    {
        { to_string(numerical_type::unknown), numerical_type::unknown },
        { to_string(numerical_type::int8), numerical_type::int8 },
        { to_string(numerical_type::uint8), numerical_type::uint8 },
        { to_string(numerical_type::int16), numerical_type::int16 },
        { to_string(numerical_type::uint16), numerical_type::uint16 },
        { to_string(numerical_type::int32), numerical_type::int32 },
        { to_string(numerical_type::uint32), numerical_type::uint32 },
        { to_string(numerical_type::int64), numerical_type::int64 },
        { to_string(numerical_type::uint64), numerical_type::uint64 },
        { to_string(numerical_type::float16), numerical_type::float16 },
        { to_string(numerical_type::brain_float16), numerical_type::brain_float16 },
        { to_string(numerical_type::float32), numerical_type::float32 },
        { to_string(numerical_type::float64), numerical_type::float64 },
        { to_string(numerical_type::complex_float16), numerical_type::complex_float16 },
        { to_string(numerical_type::complex_float32), numerical_type::complex_float32 },
        { to_string(numerical_type::complex_float64), numerical_type::complex_float64 }
    };

    const auto ite = str_2_numerical_type.find(str);
    const bool result = ite != str_2_numerical_type.end();
    if(result)
        type = ite->second;
    return result;
}

} // namespace compute
} // namespace xmipp4
