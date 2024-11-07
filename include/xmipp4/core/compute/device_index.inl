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
 * @file device_index.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of device_index.hpp
 * @date 2024-05-15
 * 
 */

#include "device_index.hpp"

#include <tuple>
#include <algorithm>
#include <cctype>

namespace xmipp4 
{
namespace compute
{

template <typename Str>
inline
device_index::device_index(Str &&backend_name, std::size_t device_id)
    : m_backend_name(std::forward<Str>(backend_name))
    , m_device_id(device_id)
{
}

template <typename Str>
inline
void device_index::set_backend_name(Str &&backend_name)
{
    m_backend_name = std::forward<Str>(backend_name);
}

inline
const std::string& device_index::get_backend_name() const noexcept
{
    return m_backend_name;
}

inline
void device_index::set_device_id(std::size_t device_index) noexcept
{
    m_device_id = device_index;
}

inline
std::size_t device_index::get_device_id() const noexcept
{
    return m_device_id;
}



inline
bool operator==(const device_index &lhs, const device_index &rhs) noexcept
{
    return lhs.get_device_id() == rhs.get_device_id() &&
           lhs.get_backend_name() == rhs.get_backend_name() ;
}

inline
bool operator!=(const device_index &lhs, const device_index &rhs) noexcept
{
    return lhs.get_device_id() != rhs.get_device_id() ||
           lhs.get_backend_name() != rhs.get_backend_name() ;
}

inline
bool operator<(const device_index &lhs, const device_index &rhs) noexcept
{
    bool result = false;

    if (lhs.get_backend_name() < rhs.get_backend_name())
    {
        result = true;
    }
    else if(lhs.get_backend_name() == rhs.get_backend_name())
    {
        result = lhs.get_device_id() < rhs.get_device_id();
    }

    return result;
}

inline
bool operator<=(const device_index &lhs, const device_index &rhs) noexcept
{
    bool result = false;

    if (lhs.get_backend_name() < rhs.get_backend_name())
    {
        result = true;
    }
    else if(lhs.get_backend_name() == rhs.get_backend_name())
    {
        result = lhs.get_device_id() <= rhs.get_device_id();
    }

    return result;
}

inline
bool operator>(const device_index &lhs, const device_index &rhs) noexcept
{
    return rhs < lhs;
}

inline
bool operator>=(const device_index &lhs, const device_index &rhs) noexcept
{
    return rhs <= lhs;
}

template <typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, const device_index &index)
{
    XMIPP4_CONST_CONSTEXPR T separator = ':';
    return os << index.get_backend_name() << separator << index.get_device_id();
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
bool parse_device_index(std::string_view path, device_index &result)
{
    bool success = false;

    XMIPP4_CONST_CONSTEXPR auto separator = ':';
    const auto ite = std::find(path.cbegin(), path.cend(), separator);
    if (ite != path.cend())
    {
        std::size_t id;
        if (detail::parse_base10_size_t(std::next(ite), path.cend(), id) == path.cend())
        {
            result = device_index(
                std::string(path.cbegin(), ite), 
                id
            );
            success = true;
        }
    }
    else
    {
        result = device_index(path, 0);
        success = true;
    }

    return success;
}

} // namespace compute
} // namespace xmipp4
