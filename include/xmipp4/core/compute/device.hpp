// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

class device_queue;
class device_memory_allocator;
class host_memory_allocator;
class device_to_host_transfer;
class host_to_device_transfer;
class device_copy;
class device_event;
class device_to_host_event;



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
     * @brief Create a device queue.
     * 
     * @return std::shared_ptr<device_queue> 
     */
    virtual std::shared_ptr<device_queue>
    create_device_queue() = 0;

    /**
     * @brief Create a memory allocator for this device.
     * 
     * @return std::shared_ptr<device_memory_allocator> 
     */
    virtual std::shared_ptr<device_memory_allocator> 
    create_device_memory_allocator() = 0;

    /**
     * @brief Create a memory allocator for the host.
     * 
     * @return std::shared_ptr<host_memory_allocator> 
     */
    virtual std::shared_ptr<host_memory_allocator> 
    create_host_memory_allocator() = 0;

    /**
     * @brief Create a host to device transfer engine.
     * 
     * @return std::shared_ptr<host_to_device_transfer> 
     */
    virtual std::shared_ptr<host_to_device_transfer> 
    create_host_to_device_transfer() = 0;

    /**
     * @brief Create a device to host transfer engine.
     * 
     * @return std::shared_ptr<device_to_host_transfer> 
     */
    virtual std::shared_ptr<device_to_host_transfer> 
    create_device_to_host_transfer() = 0;

    /**
     * @brief Create a device buffer copy engine.
     * 
     * @return std::shared_ptr<device_copy> 
     */
    virtual std::shared_ptr<device_copy> 
    create_device_copy() = 0;

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
