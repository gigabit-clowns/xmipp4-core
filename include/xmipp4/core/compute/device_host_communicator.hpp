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
 * @file device_host_communicator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device_host_communicator interface
 * @date 2024-11-06
 * 
 */

#include <memory>

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

class device_queue;
class device_buffer;
class host_buffer;
class device_memory_allocator;
class host_memory_allocator;



/**
 * @brief Abstract representation of a host to device transfer engine.
 * 
 * This class enables transferring data between host and device.
 * 
 */
class XMIPP4_CORE_API device_host_communicator
{
public:
    device_host_communicator() = default;
    device_host_communicator(const device_host_communicator &other) = default;
    device_host_communicator(device_host_communicator &&other) = default;
    virtual ~device_host_communicator() = default;

    device_host_communicator&
    operator=(const device_host_communicator &other) = default;
    device_host_communicator&
    operator=(device_host_communicator &&other) = default;

    /**
     * @brief Copy a buffer from the host to the device.
     * 
     * @param src_buffer to be transferred from.
     * @param dst_buffer to be transferred to.
     * @param queue Queue where the task will be enqueued.
     * 
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * 
     */
    virtual void 
    host_to_device(const host_buffer &src_buffer, 
                   device_buffer &dst_buffer, 
                   device_queue &queue ) const = 0;

    /**
     * @brief Transfer a buffer from the host to the device.
     * 
     * @param buffer Buffer to be transferred.
     * @param allocator Allocator from which device memory will be requested.
     * @param queue Queue where the task will be enqueued.
     * @return std::shared_ptr<device_buffer> Host buffer copied to the device.
     * 
     * @note When possible, this function will try to use a zero copy strategy
     * such that no extra memory is requested from the allocator. In that case,
     * the resulting buffer aliases the input buffer.
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * 
     */
    virtual std::shared_ptr<device_buffer> 
    host_to_device_nocopy(const std::shared_ptr<host_buffer> &buffer, 
                          device_memory_allocator &allocator,
                          device_queue &queue ) const = 0;

    /**
     * @brief Transfer a buffer from the host to the device.
     * 
     * @param buffer Buffer to be transferred.
     * @param allocator Allocator from which device memory will be requested.
     * @param queue Queue where the task will be enqueued.
     * @return std::shared_ptr<device_buffer> Host buffer copied to the device.
     * 
     * @note When possible, this function will try to use a zero copy strategy
     * such that no extra memory is requested from the allocator. In that case,
     * the resulting buffer aliases the input buffer.
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * 
     */
    virtual std::shared_ptr<const device_buffer> 
    host_to_device_nocopy(const std::shared_ptr<const host_buffer> &buffer, 
                          device_memory_allocator &allocator,
                          device_queue &queue ) const = 0;

    /**
     * @brief Copy a buffer from the host to the device.
     * 
     * @param src_buffer to be transferred from.
     * @param dst_buffer to be transferred to.
     * @param queue Queue where the task will be enqueued.
     * 
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * 
     */
    virtual void 
    device_to_host(const device_buffer &src_buffer, 
                   host_buffer &dst_buffer, 
                   device_queue &queue ) const = 0;

    /**
     * @brief Transfer a buffer from the device to the host.
     * 
     * @param buffer Buffer to be transferred.
     * @param allocator Allocator from which host memory will be requested.
     * @param queue Queue where the task will be enqueued.
     * @return std::shared_ptr<host_buffer> Device buffer copied to the host.
     * 
     * @note When possible, this function will try to use a zero copy strategy
     * such that no extra memory is requested from the allocator. In that case,
     * the resulting buffer aliases the input buffer.
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * 
     */
    virtual std::shared_ptr<host_buffer> 
    device_to_host_nocopy(const std::shared_ptr<device_buffer> &buffer, 
                          host_memory_allocator &allocator,
                          device_queue &queue ) const = 0;

    /**
     * @brief Transfer a buffer from the device to the host.
     * 
     * @param buffer Buffer to be transferred.
     * @param allocator Allocator from which host memory will be requested.
     * @param queue Queue where the task will be enqueued.
     * @return std::shared_ptr<host_buffer> Device buffer copied to the host.
     * 
     * @note When possible, this function will try to use a zero copy strategy
     * such that no extra memory is requested from the allocator. In that case,
     * the resulting buffer aliases the input buffer.
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * 
     */
    virtual std::shared_ptr<const host_buffer> 
    device_to_host_nocopy(const std::shared_ptr<const device_buffer> &buffer, 
                          host_memory_allocator &allocator,
                          device_queue &queue ) const = 0;

}; 

} // namespace compute
} // namespace xmipp4
