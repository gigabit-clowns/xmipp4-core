// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>
#include <vector>

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

class device_queue;
class device_event;
class device_to_host_event;
class memory_resource;



/**
 * @brief Abstract representation of a device handle.
 * 
 * This class enables basic interaction functionalities with
 * a compute device.
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
     * @brief Enumerates all the memory resources accessible by this device.
     * 
     * @param resources Output parameter where resources are written.
     */
    virtual
    void enumerate_memory_resources(
        std::vector<std::shared_ptr<memory_resource>> &resources
    ) = 0;

    /**
     * @brief Create a device queue.
     * 
     * @return std::shared_ptr<device_queue> 
     */
    virtual std::shared_ptr<device_queue>
    create_device_queue() = 0;

    /**
     * @brief Create an intra-device synchronization primitive.
     * 
     * @return std::shared_ptr<device_event> 
     */
    virtual std::shared_ptr<device_event>
    create_device_event() = 0;

    /**
     * @brief Create a device to host synchronization primitive.
     * 
     * @return std::shared_ptr<device_to_host_event> 
     */
    virtual std::shared_ptr<device_to_host_event>
    create_device_to_host_event() = 0;

}; 

} // namespace compute
} // namespace xmipp4
