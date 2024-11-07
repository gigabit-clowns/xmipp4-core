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
 * @file host_to_device_transfer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::host_to_device_transfer interface
 * @date 2024-11-06
 * 
 */

#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace compute
{

class device_queue;
class device_buffer;
class host_buffer;
class device_memory_allocator;



/**
 * @brief Abstract representation of a host to device transfer engine.
 * 
 * This class enables transferring data from host to device.
 * 
 */
class XMIPP4_CORE_API host_to_device_transfer
{
public:
    host_to_device_transfer() = default;
    host_to_device_transfer(const host_to_device_transfer &other) = default;
    host_to_device_transfer(host_to_device_transfer &&other) = default;
    virtual ~host_to_device_transfer() = default;

    host_to_device_transfer&
    operator=(const host_to_device_transfer &other) = default;
    host_to_device_transfer&
    operator=(host_to_device_transfer &&other) = default;



    /**
     * @brief Transfer a buffer from the host to the device.
     * 
     * @param src_buffer Buffer to be transferred from.
     * @param dst_buffer Buffer to be transferred to.
     * @param queue Queue where the task will be enqueued.
     * 
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * @note src_buffer is required to be a shared_ptr as host buffers are
     * not stream synchronized. Thus, a copy of the buffer is retained
     * until the transfer has completed to prevent premature deallocation.
     * 
     */
    virtual void
    transfer(const std::shared_ptr<const host_buffer> &src_buffer,
             device_buffer &dst_buffer, 
             device_queue &queue ) const = 0;


    /**
     * @brief Transfer a buffer from the host to the device.
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
    virtual std::shared_ptr<device_buffer> 
    transfer_nocopy(const std::shared_ptr<host_buffer> &buffer, 
                    device_memory_allocator &allocator,
                    device_queue &queue ) const = 0;

    /**
     * @brief Transfer a buffer from the host to the device.
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
    virtual std::shared_ptr<const device_buffer> 
    transfer_nocopy(const std::shared_ptr<const host_buffer> &buffer, 
                    device_memory_allocator &allocator,
                    device_queue &queue ) const = 0;

    /**
     * @brief Block the current thread until the transfers have finished.
     * 
     */
    virtual void wait() = 0;

    /**
     * @brief Block the provided queue until the transfer has finished 
     * 
     * @param queue The queue to be blocked.
     * 
     * @note The queue provided to the transfer is implicitly blocked.
     * This function may not be used with the same queue used for the 
     * transfer. Instead, this function is meant to be used with other
     * queues that await the transferred data.
     * 
     */
    virtual void wait(device_queue &queue) = 0;

}; 

} // namespace compute
} // namespace xmipp4
