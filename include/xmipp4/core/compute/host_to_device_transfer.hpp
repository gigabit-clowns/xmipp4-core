// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file host_to_device_transfer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::host_to_device_transfer interface
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
    transfer_copy(const host_buffer &src_buffer,
                  device_buffer &dst_buffer, 
                  device_queue &queue ) = 0;

    /**
     * @brief Transfer regions buffer from the host to the device.
     * 
     * @param src_buffer Buffer to be transferred from.
     * @param dst_buffer Buffer to be transferred to.
     * @param queue Queue where the task will be enqueued.
     * @param regions Regions to be copied.
     * 
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * @note src_buffer is required to be a shared_ptr as host buffers are
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
    transfer_copy(const host_buffer &src_buffer,
                  device_buffer &dst_buffer,
                  span<const copy_region> regions,
                  device_queue &queue ) = 0;

    /**
     * @brief Transfer a buffer from the host to the device.
     * 
     * @param buffer Buffer to be transferred.
     * @param allocator Allocator from which host memory will be requested.
     * @param alignment Alignment used for allocating a new buffer.
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
    transfer(const std::shared_ptr<host_buffer> &buffer, 
             device_memory_allocator &allocator,
             std::size_t alignment,
             device_queue &queue ) = 0;

    /**
     * @brief Transfer a buffer from the host to the device.
     * 
     * @param buffer Buffer to be transferred.
     * @param allocator Allocator from which host memory will be requested.
     * @param alignment Alignment used for allocating a new buffer.
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
    transfer(const std::shared_ptr<const host_buffer> &buffer, 
             device_memory_allocator &allocator,
             std::size_t alignment,
             device_queue &queue ) = 0;

}; 

} // namespace compute
} // namespace xmipp4
