// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_index.hpp"
#include "device_properties.hpp"
#include "device_backend.hpp"
#include "../service_manager.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace hardware
{

class device;
class device_backend;
class device_create_parameters;



/**
 * @brief Centralize multiple device_backends.
 * 
 */
class device_manager final
    : public basic_service_manager<device_backend>
{
public:
    XMIPP4_CORE_API device_manager();
    device_manager(const device_manager &other) = delete;
    XMIPP4_CORE_API device_manager(device_manager &&other) noexcept;
    XMIPP4_CORE_API ~device_manager() override;

    device_manager& operator=(const device_manager &other) = delete;
    XMIPP4_CORE_API device_manager& operator=(device_manager &&other) noexcept;

    XMIPP4_CORE_API
    void register_builtin_backends() override;

    /**
     * @brief Enumerate devices across all backends.
     * 
     * @param indices Output list of device indices.
     * @note The output list is cleared before populating it.
     * 
     */
    XMIPP4_CORE_API
    void enumerate_devices(std::vector<device_index> &indices) const;

    /**
     * @brief Query the properties of a device.
     * 
     * @param index The index of the device.
     * @param desc Output device properties.
     * @return true Device exists and properties were written.
     * @return false Device does not exist and properties were not written.
     */
    XMIPP4_CORE_API
    bool get_device_properties(const device_index &index, 
                               device_properties &desc ) const;

    /**
     * @brief Create a device handle.
     * 
     * @param index Index of the device.
     * @param params Parameters used for device instantiation.
     * @return std::shared_ptr<device> The device handle.
     */
    XMIPP4_CORE_API 
    std::shared_ptr<device> create_device(const device_index &index) const;

}; 

} // namespace hardware
} // namespace xmipp4
