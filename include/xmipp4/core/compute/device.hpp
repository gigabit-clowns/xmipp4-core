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

/**
 * @file device.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device interface
 * @date 2024-10-22
 * 
 */

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
class device_buffer_copy;



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
     * @brief Create a command queue for this device.
     * 
     * @return std::unique_ptr<device_queue> The queue.
     */
    virtual std::unique_ptr<device_queue> create_queue() = 0;

    /**
     * @brief Create a command queue for this device.
     * 
     * @return std::shared_ptr<device_queue> The queue.
     */
    virtual std::shared_ptr<device_queue> create_queue_shared() = 0;

    /**
     * @brief Create a memory allocator for this device.
     * 
     * @return std::unique_ptr<device_memory_allocator> 
     */
    virtual std::unique_ptr<device_memory_allocator> 
    create_device_memory_allocator() = 0;

    /**
     * @brief Create a memory allocator for this device.
     * 
     * @return std::shared_ptr<device_memory_allocator> 
     */
    virtual std::shared_ptr<device_memory_allocator> 
    create_device_memory_allocator_shared() = 0;

    /**
     * @brief Create a memory allocator for the host.
     * 
     * @return std::unique_ptr<host_memory_allocator> 
     */
    virtual std::unique_ptr<host_memory_allocator> 
    create_host_memory_allocator() = 0;

    /**
     * @brief Create a memory allocator for the host.
     * 
     * @return std::shared_ptr<host_memory_allocator> 
     */
    virtual std::shared_ptr<host_memory_allocator> 
    create_host_memory_allocator_shared() = 0;

    /**
     * @brief Create a host to device transfer engine.
     * 
     * @return std::unique_ptr<host_to_device_transfer> 
     */
    virtual std::unique_ptr<host_to_device_transfer> 
    create_host_to_device_transfer() = 0;

    /**
     * @brief Create a host to device transfer engine.
     * 
     * @return std::shared_ptr<host_to_device_transfer> 
     */
    virtual std::shared_ptr<host_to_device_transfer> 
    create_host_to_device_transfer_shared() = 0;

    /**
     * @brief Create a device to host transfer engine.
     * 
     * @return std::unique_ptr<device_to_host_transfer> 
     */
    virtual std::unique_ptr<device_to_host_transfer> 
    create_device_to_host_transfer() = 0;

    /**
     * @brief Create a device to host transfer engine.
     * 
     * @return std::shared_ptr<device_to_host_transfer> 
     */
    virtual std::shared_ptr<device_to_host_transfer> 
    create_device_to_host_transfer_shared() = 0;

    /**
     * @brief Create a device buffer copy engine.
     * 
     * @return std::unique_ptr<device_copy> 
     */
    virtual std::unique_ptr<device_buffer_copy> 
    create_device_buffer_copy() = 0;

    /**
     * @brief Create a device buffer copy engine.
     * 
     * @return std::shared_ptr<device_copy> 
     */
    virtual std::shared_ptr<device_buffer_copy> 
    create_device_buffer_copy_shared() = 0;

}; 

} // namespace compute
} // namespace xmipp4
