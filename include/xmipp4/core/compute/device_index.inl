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
#include <charconv>

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

inline
const std::string& device_index::get_backend_name() const noexcept
{
    return m_backend_name;
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



namespace detail
{

XMIPP4_INLINE_CONSTEXPR char get_device_index_separator() noexcept
{
    return ':';
}

} // namespace detail



template <typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, const device_index &index)
{
    XMIPP4_CONST_CONSTEXPR auto separator = 
        detail::get_device_index_separator();
    return os << index.get_backend_name() << separator << index.get_device_id();
}

inline
bool parse_device_index(std::string_view text, device_index &result)
{    
    if(text.empty())
    {
        return false;
    }

    XMIPP4_CONST_CONSTEXPR auto separator = 
        detail::get_device_index_separator();

    bool success = false;
    const auto begin = text.data();
    const auto end = begin + text.size();
    const auto ite = std::find(begin, end, separator);
    if (ite == end)
    {
        result = device_index(text, 0);
        success = true;
    }
    else if (ite != begin)
    {
        std::size_t id;
        if (std::from_chars(std::next(ite), end, id, 10).ec == std::errc())
        {
            result = device_index(
                std::string(begin, ite), 
                id
            );
            success = true;
        }
    }

    return success;
}

} // namespace compute
} // namespace xmipp4
