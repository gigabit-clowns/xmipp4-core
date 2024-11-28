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
 * @file device_to_host_transfer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device_to_host_transfer interface
 * @date 2024-11-06
 * 
 */

#include "copy_region.hpp"
#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace compute
{

class device_queue;
class device_buffer;
class host_buffer;
class host_memory_allocator;



/**
 * @brief Abstract representation of a device to host transfer engine.
 * 
 * This class enables transferring data from device to host.
 * 
 */
class XMIPP4_CORE_API device_to_host_transfer
{
public:
    device_to_host_transfer() = default;
    device_to_host_transfer(const device_to_host_transfer &other) = default;
    device_to_host_transfer(device_to_host_transfer &&other) = default;
    virtual ~device_to_host_transfer() = default;

    device_to_host_transfer&
    operator=(const device_to_host_transfer &other) = default;
    device_to_host_transfer&
    operator=(device_to_host_transfer &&other) = default;



    /**
     * @brief Transfer a buffer from the device to the host.
     * 
     * @param src_buffer Buffer to be transferred from.
     * @param dst_buffer Buffer to be transferred to.
     * @param queue Queue where the task will be enqueued.
     * 
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * @note dst_buffer is required to be a shared_ptr as host buffers are
     * not stream synchronized. Thus, a copy of the buffer is retained
     * until the transfer has completed to prevent premature deallocation.
     * @note Both buffers must have the same numerical type and element counts.
     * 
     */
    virtual void
    transfer_copy(const device_buffer &src_buffer,
                  host_buffer &dst_buffer,
                  device_queue &queue ) = 0;

    /**
     * @brief Transfer regions buffer from the device to the host.
     * 
     * @param src_buffer Buffer to be transferred from.
     * @param dst_buffer Buffer to be transferred to.
     * @param queue Queue where the task will be enqueued.
     * @param regions Regions to be copied.
     * 
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * @note dst_buffer is required to be a shared_ptr as host buffers are
     * not stream synchronized. Thus, a copy of the buffer is retained
     * until the transfer has completed to prevent premature deallocation.
     * @note All values in regions are expressed in terms of element counts
     * @note Both buffers must have the same numerical type.
     * @note For all regions, source_offset+count must not be greater
     * than the source buffer size and destination_offset+count
     * must not be greater than the destination buffer size.
     * @note Regions should not overlap within a domain.
     * 
     */
    virtual void
    transfer_copy(const device_buffer &src_buffer,
                  host_buffer &dst_buffer,
                  span<const copy_region> regions,
                  device_queue &queue ) = 0;

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
    transfer(const std::shared_ptr<device_buffer> &buffer, 
             host_memory_allocator &allocator,
             device_queue &queue ) = 0;

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
    transfer(const std::shared_ptr<const device_buffer> &buffer, 
             host_memory_allocator &allocator,
             device_queue &queue ) = 0;

}; 

} // namespace compute
} // namespace xmipp4
