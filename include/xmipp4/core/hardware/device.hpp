// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>
#include <vector>

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class device_queue;
class device_event;
class device_to_host_event;
class memory_resource;


/**
 * @brief Abstract representation of a device handle.
 * 
 * This class enables basic interaction functionalities with
 * a hardware device.
 * 
 */
class XMIPP4_CORE_API device
{
public:
    device() = default;
    device(const device &other) = default;
    device(device &&other) = default;
    virtual ~device() = default;

    device& operator=(const device &other) = default;
    device& operator=(device &&other) = default;

    /**
     * @brief Enumerates all the memory resources known by this device.
     * 
     * @param resources Output parameter where resources are written. The 
     * resources are owned by this device object and the caller should not 
     * attempt to free free them.
     */
    virtual
    void enumerate_memory_resources(
        std::vector<memory_resource*> &resources // Evaluate output type
    ) = 0;

    /**
     * @brief Evaluates whether this device can access the given memory 
     * resource.
     * 
     * Certain memory_resource-s are accessible by definition. This includes
     * any memory_resource enumerated by this device whose kind satisfies
     * is_device_accessible. However, some other memory_resource-s can
     * also be accessible. This method is suited to query those cases in
     * runtime.
     * 
     * @param resource The resource to evaluated.
     * @return true If the device can access the resource.
     * @return false If the device cannot access the resource.
     */
    virtual 
    bool can_access_memory_resource(const memory_resource &resource) const = 0;

    /**
     * @brief Create a device queue.
     * 
     * @return std::shared_ptr<device_queue> The created device queue.
     */
    virtual std::shared_ptr<device_queue>
    create_device_queue() = 0;

    /**
     * @brief Create an intra-device synchronization primitive.
     * 
     * @return std::shared_ptr<device_event> The created device event.
     */
    virtual std::shared_ptr<device_event>
    create_device_event() = 0;

    /**
     * @brief Create a device to host synchronization primitive.
     * 
     * @return std::shared_ptr<device_to_host_event> The created 
     * device_to_host_event.
     */
    virtual std::shared_ptr<device_to_host_event>
    create_device_to_host_event() = 0;

}; 

} // namespace hardware
} // namespace xmipp4
