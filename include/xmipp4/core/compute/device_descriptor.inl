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

void device_descriptor::set_uuid(const uuid &id) noexcept
{
    m_uuid = id;
}

const uuid& device_descriptor::get_uuid() const noexcept
{
    return m_uuid;
}

void device_descriptor::set_type(device_type type) noexcept
{
	m_type = type;
}

device_type device_descriptor::get_type() const noexcept
{
    return m_type;
}

template <typename Str>
void device_descriptor::set_vendor(Str &&vendor)
{
    m_vendor = std::forward<Str>(vendor);
}

const std::string& device_descriptor::get_vendor() const noexcept
{
    return m_vendor;
}

template <typename Str>
void device_descriptor::set_model(Str &&model)
{
    m_model = std::forward<Str>(model);
}

const std::string& device_descriptor::get_model() const noexcept
{
    return m_model;
}

}
} // namespace xmipp4
