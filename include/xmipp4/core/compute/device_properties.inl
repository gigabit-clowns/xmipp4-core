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

#include "device_properties.hpp"

#include <utility>

namespace xmipp4
{
namespace compute
{

inline
void device_properties::set_type(device_type type) noexcept
{
	m_type = type;
}

inline
device_type device_properties::get_type() const noexcept
{
    return m_type;
}

template <typename Str>
inline
void device_properties::set_name(Str &&name)
{
    m_name = std::forward<Str>(name);
}

inline
const std::string& device_properties::get_name() const noexcept
{
    return m_name;
}

template <typename Str>
inline
void device_properties::set_physical_location(Str &&location)
{
    m_physical_location = std::forward<Str>(location);
}

inline
const std::string& device_properties::get_physical_location() const noexcept
{
    return m_physical_location;
}

inline
void device_properties::set_total_memory_bytes(std::size_t bytes) noexcept
{
    m_total_memory_bytes = bytes;
}

inline
std::size_t device_properties::get_total_memory_bytes() const noexcept
{
    return m_total_memory_bytes;
}

} // namespace compute
} // namespace xmipp4
