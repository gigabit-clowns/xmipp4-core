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

#include "device_type.hpp"
#include "../uuid.hpp"
#include "../platform/constexpr.hpp"

#include <string>

namespace xmipp4
{
namespace compute
{

class device_descriptor
{
public:
    device_descriptor();
    device_descriptor(const device_descriptor& other) = default;
    device_descriptor(device_descriptor&& other) = default;
    ~device_descriptor() = default;

    device_descriptor& operator=(const device_descriptor& other) = default;
    device_descriptor& operator=(device_descriptor&& other) = default;

    void set_uuid(const uuid &id) noexcept;
    const uuid& get_uuid() const noexcept;

    void set_type(device_type type) noexcept;
    device_type get_type() const noexcept;

    template <typename Str>
    void set_vendor(Str &&vendor);
    const std::string& get_vendor() const noexcept;

    template <typename Str>
    void set_model(Str &&model);
    const std::string& get_model() const noexcept;

    template <typename Str>
    void set_physical_location(Str &&location);
    const std::string& get_physical_location() const noexcept;

private:
    uuid m_uuid;
    device_type m_type;
    std::string m_vendor;
    std::string m_model;
    std::string m_physical_location;

};

bool operator==(const device_descriptor &lhs, const device_descriptor &rhs) noexcept;
bool operator!=(const device_descriptor &lhs, const device_descriptor &rhs) noexcept;
bool operator<(const device_descriptor &lhs, const device_descriptor &rhs) noexcept;
bool operator<=(const device_descriptor &lhs, const device_descriptor &rhs) noexcept;
bool operator>(const device_descriptor &lhs, const device_descriptor &rhs) noexcept;
bool operator>=(const device_descriptor &lhs, const device_descriptor &rhs) noexcept;

} // namespace compute
} // namespace xmipp4

#include "device_descriptor.inl"
