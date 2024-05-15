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
 * @file location.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of location.hpp
 * @date 2024-05-15
 * 
 */

#include "location.hpp"

#include <algorithm>
#include <cctype>

namespace xmipp4 
{
namespace image
{

template <typename Str>
inline
location::location(Str &&filename, std::size_t position)
    : m_filename(std::forward<Str>(filename))
    , m_position(position)
{
}

template <typename Str>
inline
void location::set_filename(Str &&filename)
{
    m_filename = std::forward<Str>(filename);
}

inline
const std::string& location::get_filename() const noexcept
{
    return m_filename;
}

inline
void location::set_position(std::size_t position) noexcept
{
    m_position = position;
}

inline
std::size_t location::get_position() const noexcept
{
    return m_position;
}



namespace detail
{

template <typename ForwardIt>
inline
ForwardIt parse_base10_size_t(ForwardIt first, ForwardIt last, std::size_t &result)
{
    XMIPP4_CONST_CONSTEXPR auto base = 10;

    result = 0;
    while(first != last)
    {
        if(!std::isdigit(*first))
        {
            break;
        }
        
        // Obtain the digit from the character
        const auto digit = static_cast<std::size_t>(*first - '0');

        // Add to the result
        result *= base;
        result += digit;

        ++first;
    }

    return first;
}

} // namespace detail

inline
bool parse_location(const std::string &path, location &result)
{
    bool success = false;

    XMIPP4_CONST_CONSTEXPR auto separator = '@';
    const auto ite = std::find(path.cbegin(), path.cend(), separator);
    if (ite != path.cend())
    {
        // Filename and position
        std::size_t position;
        if (detail::parse_base10_size_t(path.cbegin(), ite, position) == ite)
        {
            result = location(
                std::string(std::next(ite), path.cend()), 
                position
            );
            success = true;
        }
    }   
    else
    {
        // Only filename
        result = location(path, location::no_position);
        success = true;
    }

    return success;
}

template <typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, const location &loc)
{
    if(loc.get_position() != location::no_position)
    {
        XMIPP4_CONST_CONSTEXPR T separator = '@';
        os << loc.get_position() << separator;

    }

    return os << loc.get_filename();
}

} // namespace image
} // namespace xmipp4
