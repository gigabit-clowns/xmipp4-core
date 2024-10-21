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

#include "device_descriptor.hpp"

#include <utility>

namespace xmipp4
{
namespace compute
{

inline
void device_descriptor::set_uuid(const uuid &id) noexcept
{
    m_uuid = id;
}

inline
const uuid& device_descriptor::get_uuid() const noexcept
{
    return m_uuid;
}

inline
void device_descriptor::set_type(device_type type) noexcept
{
	m_type = type;
}

inline
device_type device_descriptor::get_type() const noexcept
{
    return m_type;
}

inline
template <typename Str>
void device_descriptor::set_vendor(Str &&vendor)
{
    m_vendor = std::forward<Str>(vendor);
}

inline
const std::string& device_descriptor::get_vendor() const noexcept
{
    return m_vendor;
}

inline
template <typename Str>
void device_descriptor::set_model(Str &&model)
{
    m_model = std::forward<Str>(model);
}

inline
const std::string& device_descriptor::get_model() const noexcept
{
    return m_model;
}

inline
template <typename Str>
void device_descriptor::set_physical_location(Str &&location)
{
    m_physical_location = std::forward<Str>(location);
}

inline
const std::string& device_descriptor::get_physical_location() const noexcept
{
    return m_physical_location;
}



inline
bool operator==(const device_descriptor &lhs, const device_descriptor &rhs) noexcept
{
    return lhs.get_uuid() == rhs.get_uuid();
}

inline
bool operator!=(const device_descriptor &lhs, const device_descriptor &rhs) noexcept
{
    return lhs.get_uuid() != rhs.get_uuid();
}

inline
bool operator<(const device_descriptor &lhs, const device_descriptor &rhs) noexcept
{
    return lhs.get_uuid() < rhs.get_uuid();
}

inline
bool operator<=(const device_descriptor &lhs, const device_descriptor &rhs) noexcept
{
    return lhs.get_uuid() <= rhs.get_uuid();
}

inline
bool operator>(const device_descriptor &lhs, const device_descriptor &rhs) noexcept
{
    return lhs.get_uuid() > rhs.get_uuid();
}

inline
bool operator>=(const device_descriptor &lhs, const device_descriptor &rhs) noexcept
{
    return lhs.get_uuid() >= rhs.get_uuid();
}

}
} // namespace xmipp4
