// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file device_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines device_backend interface
 * @date 2024-10-23
 * 
 */

#include "device_properties.hpp"
#include "../backend.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace compute
{

class device;
class device_create_parameters;

/**
 * @brief Abstract class that represents a compute backend.
 * 
 * This interface provides basic methods for querying and
 * obtaining handles to compute devices.
 * 
 */
class XMIPP4_CORE_API device_backend
    : public backend
{
public:
    device_backend() = default;
    device_backend(const device_backend &other) = default;
    device_backend(device_backend &&other) = default;
    virtual ~device_backend() = default;

    device_backend& operator=(const device_backend &other) = default;
    device_backend& operator=(device_backend &&other) = default;

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
    virtual bool get_device_properties(std::size_t id, device_properties &desc) const = 0;

    /**
     * @brief Create a device handle for the given device identifier.
     * 
     * @param id The identifier of the device.
     * @param params Parameters used for device instantiation.
     * @return std::shared_ptr<device> The device handle.
     */
    virtual std::shared_ptr<device> 
    create_device(std::size_t id, const device_create_parameters &params) = 0;

}; 

} // namespace compute
} // namespace xmipp4
