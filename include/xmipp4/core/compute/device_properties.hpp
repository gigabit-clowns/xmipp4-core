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
#include "../platform/constexpr.hpp"

#include <string>

namespace xmipp4
{
namespace compute
{

class device_properties
{
public:
    device_properties();
    device_properties(const device_properties& other) = default;
    device_properties(device_properties&& other) = default;
    ~device_properties() = default;

    device_properties& operator=(const device_properties& other) = default;
    device_properties& operator=(device_properties&& other) = default;

    void set_type(device_type type) noexcept;
    device_type get_type() const noexcept;

    template <typename Str>
    void set_name(Str &&name);
    const std::string& get_name() const noexcept;

    template <typename Str>
    void set_physical_location(Str &&location);
    const std::string& get_physical_location() const noexcept;

    void set_total_memory_bytes(std::size_t bytes) noexcept;
    std::size_t get_total_memory_bytes() const noexcept;

private:
    device_type m_type;
    std::string m_name;
    std::string m_physical_location;
    std::size_t m_total_memory_bytes;

};

} // namespace compute
} // namespace xmipp4

#include "device_properties.inl"
