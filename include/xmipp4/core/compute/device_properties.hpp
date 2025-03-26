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

/**
 * @brief Description of the characteristics of a physical device
 * in the host.
 * 
 */
class device_properties
{
public:
    device_properties();
    device_properties(const device_properties& other) = default;
    device_properties(device_properties&& other) = default;
    ~device_properties() = default;

    device_properties& operator=(const device_properties& other) = default;
    device_properties& operator=(device_properties&& other) = default;

    /**
     * @brief Set the device type.
     * 
     * @param type The device type.
     */
    void set_type(device_type type) noexcept;

    /**
     * @brief Get the device type.
     * 
     * @return device_type The device type. 
     */
    device_type get_type() const noexcept;

    /**
     * @brief Set the device name.
     * 
     * @tparam Str String type.
     * @param name The device name.
     */
    template <typename Str>
    void set_name(Str &&name);

    /**
     * @brief Get the device name.
     * 
     * @return const std::string& The device name.
     */
    const std::string& get_name() const noexcept;

    /**
     * @brief Set the physical location of the device.
     * 
     * Usually this is a PCI address.
     * 
     * @tparam Str String type.
     * @param location The physical location of the device.
     */
    template <typename Str>
    void set_physical_location(Str &&location);

    /**
     * @brief Get the physical location of the device.
     * 
     * Usually this is a PCI address.
     * 
     * @return const std::string& The physical location of the device.
     */
    const std::string& get_physical_location() const noexcept;

    /**
     * @brief Set the total memory of the device in bytes.
     * 
     * @param bytes Number of bytes.
     */
    void set_total_memory_bytes(std::size_t bytes) noexcept;

    /**
     * @brief Get the total memory of the device in bytes.
     * 
     * @return std::size_t Number of bytes
     */
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
