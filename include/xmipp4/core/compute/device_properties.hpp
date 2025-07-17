// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
