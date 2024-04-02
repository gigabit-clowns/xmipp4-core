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

#include "index.hpp"

#include <type_traits>
#include <stdexcept>

namespace xmipp4 
{

namespace detail
{

template <typename I>
inline
typename std::enable_if<std::is_integral<I>::value && !std::is_signed<I>::value, std::size_t>::type
sanitize_index(I index, std::size_t size)
{
    if (index >= size)
    {
        throw std::out_of_range("Index exceeds or equals array length");
    }

    return static_cast<std::size_t>(index);
}

template <typename I>
inline
typename std::enable_if<std::is_integral<I>::value && std::is_signed<I>::value, std::size_t>::type
sanitize_index(I index, std::size_t length)
{
    std::size_t result;

    if (index < 0)
    {
        if (index < (-length))
        {
            throw std::out_of_range("Index exceeds negative array length");
        }

        result = static_cast<std::size_t>(index + length);
    }
    else
    {
        using unsigned_type = typename std::make_unsigned<I>::type;
        result = sanitize_index(static_cast<unsigned_type>(index), length);
    }

    return result;
}

template <typename I, I index>
inline
std::size_t sanitize_index(std::integral_constant<I, index>, std::size_t length)
{
    return sanitize_index(index, length);
}

} // namespace detail



template <typename I>
inline
std::size_t sanitize_index(I index, std::size_t length)
{
    return detail::sanitize_index(index, length);
}

} // namespace xmipp4
