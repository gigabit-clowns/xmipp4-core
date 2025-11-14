// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_properties.hpp"
#include "../version.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace hardware
{

class device;

/**
 * @brief Abstract class that represents a hardware backend.
 * 
 * This interface provides basic methods for querying and
 * obtaining handles to hardware devices.
 * 
 */
class XMIPP4_CORE_API device_backend
{
public:
    device_backend() = default;
    device_backend(const device_backend &other) = default;
    device_backend(device_backend &&other) = default;
    virtual ~device_backend() = default;

    device_backend& operator=(const device_backend &other) = default;
    device_backend& operator=(device_backend &&other) = default;

    /**
     * @brief Get the name of the backend.
     * 
     * @return std::string The name
     */
    virtual std::string get_name() const = 0;

    /**
     * @brief Get the version of the backend.
     * 
     * @return version The version.
     */
    virtual version get_version() const = 0;

    /**
     * @brief Enumerate available in devices within this backend.
     * 
     * @param ids IDs of the devices. 
     * @note The IDs are only unique within each backend.
     * 
     */
    virtual void enumerate_devices(std::vector<std::size_t> &ids) const = 0;

    /**
     * @brief Query the properties of a device.
     * 
     * @param id Identifier of the device.
     * @param desc Output properties.
     * @return true The device ID exists. Properties were written.
     * @return false The device ID does not exist. Properties were not written.
     * 
     */
    virtual bool get_device_properties(
        std::size_t id, 
        device_properties &desc
    ) const = 0;

    /**
     * @brief Create a device handle for the given device identifier.
     * 
     * @param id The identifier of the device.
     * @return std::shared_ptr<device> The device handle.
     */
    virtual std::shared_ptr<device> create_device(std::size_t id) = 0;

}; 

} // namespace hardware
} // namespace xmipp4
