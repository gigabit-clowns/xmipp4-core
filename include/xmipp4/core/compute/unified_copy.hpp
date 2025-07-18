// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_to_host_transfer.hpp"
#include "host_to_device_transfer.hpp"
#include "device_copy.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

class unified_buffer;

/**
 * @brief Implementation of the host_to_device_transfer, device_to_host_transfer 
 * and device_copy interfaces to allow migrate data in unified architectures.
 * 
 * @note Unless the method explicitly indicates a copy, all
 * transfers a zero-copy operation, this is, the input buffer
 * is casted to the output buffer, aliasing its contents.
 * 
 */
class XMIPP4_CORE_API unified_copy
    : public host_to_device_transfer
    , public device_to_host_transfer
    , public device_copy
{
public:
    void transfer_copy(const host_buffer &src_buffer,
                       device_buffer &dst_buffer, 
                       device_queue &queue ) override;
    
    void transfer_copy(const host_buffer &src_buffer,
                       device_buffer &dst_buffer, 
                       span<const copy_region> regions, 
                       device_queue &queue ) override;

    std::shared_ptr<device_buffer> 
    transfer(const std::shared_ptr<host_buffer> &buffer, 
             device_memory_allocator &allocator,
             std::size_t alignment,
             device_queue &queue ) override;

    std::shared_ptr<const device_buffer> 
    transfer(const std::shared_ptr<const host_buffer> &buffer, 
             device_memory_allocator &allocator,
             std::size_t alignment,
             device_queue &queue ) override;

    void transfer_copy(const device_buffer &src_buffer,
                       host_buffer &dst_buffer, 
                       device_queue &queue ) override;

    void transfer_copy(const device_buffer &src_buffer,
                       host_buffer &dst_buffer, 
                       span<const copy_region> regions, 
                       device_queue &queue ) override;

    std::shared_ptr<host_buffer> 
    transfer(const std::shared_ptr<device_buffer> &buffer, 
             host_memory_allocator &allocator,
             std::size_t alignment,
             device_queue &queue ) override;

    std::shared_ptr<const host_buffer> 
    transfer(const std::shared_ptr<const device_buffer> &buffer, 
             host_memory_allocator &allocator,
             std::size_t alignment,
             device_queue &queue ) override;

    void copy(const device_buffer &src_buffer,
              device_buffer &dst_buffer, 
              device_queue &queue ) override;

    void copy(const device_buffer &src_buffer,
              device_buffer &dst_buffer,
              span<const copy_region> regions,
              device_queue &queue ) override;
    
    /**
     * @brief Copy data from one unified_buffer to another.
     * 
     * @param src_buffer Buffer to be transferred from.
     * @param dst_buffer Buffer to be transferred to.
     * @param queue Queue where the task will be enqueued.
     * 
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * @note Both buffers must have the same size.
     * @note It is highly encouraged to override this function in child classes,
     * as the default implementation blocks execution to then copy the data
     * using the CPU.
     * 
     */
    virtual
    void copy_unified(const unified_buffer &src_buffer,
                      unified_buffer &dst_buffer, 
                      device_queue &queue );

    /**
     * @brief Copy regions of data from one unified_buffer to another.
     * 
     * @param src_buffer Buffer to be transferred from.
     * @param dst_buffer Buffer to be transferred to.
     * @param regions Regions to be copied.
     * @param queue Queue where the task will be enqueued.
     * 
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * @note All values in regions are expressed in terms of element counts
     * @note For all regions, source_offset+count must not be greater
     * than the source buffer size and destination_offset+count
     * must not be greater than the destination buffer size.
     * @note Regions should not overlap within a domain.
     * @note It is highly encouraged to override this function in child classes,
     * as the default implementation blocks execution to then copy the data 
     * using the CPU.
     * 
     */
    virtual
    void copy_unified(const unified_buffer &src_buffer,
                      unified_buffer &dst_buffer,
                      span<const copy_region> regions,
                      device_queue &queue );

}; 

} // namespace compute
} // namespace xmipp4
