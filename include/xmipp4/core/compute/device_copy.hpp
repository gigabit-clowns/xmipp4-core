// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
class device_memory_allocator;



/**
 * @brief Abstract representation of an intra-device transfer engine.
 * 
 * This class enables transferring data inside a device.
 * 
 */
class XMIPP4_CORE_API device_copy
{
public:
    device_copy() = default;
    device_copy(const device_copy &other) = default;
    device_copy(device_copy &&other) = default;
    virtual ~device_copy() = default;

    device_copy&
    operator=(const device_copy &other) = default;
    device_copy&
    operator=(device_copy &&other) = default;



    /**
     * @brief Copy data from one device_buffer to another.
     * 
     * @param src_buffer Buffer to be transferred from.
     * @param dst_buffer Buffer to be transferred to.
     * @param queue Queue where the task will be enqueued.
     * 
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * @note Both buffers must have the same numerical type.
     * 
     */
    virtual void
    copy(const device_buffer &src_buffer,
         device_buffer &dst_buffer, 
         device_queue &queue ) = 0;

    /**
     * @brief Copy regions of data from one device_buffer to another.
     * 
     * @param src_buffer Buffer to be transferred from.
     * @param dst_buffer Buffer to be transferred to.
     * @param queue Queue where the task will be enqueued.
     * @param regions Regions to be copied.
     * 
     * @note Source buffer should not be modified until the operation has
     * been completed. Destination buffer should not be either read nor 
     * written until the transfer has completed.
     * @note All values in regions are expressed in terms of element counts
     * @note Both buffers must have the same numerical type.
     * @note For all regions, source_offset+count must not be greater
     * than the source buffer size and destination_offset+count
     * must not be greater than the destination buffer size.
     * @note Regions should not overlap within a domain.
     * 
     */
    virtual void
    copy(const device_buffer &src_buffer,
         device_buffer &dst_buffer,
         span<const copy_region> regions,
         device_queue &queue ) = 0;

}; 

} // namespace compute
} // namespace xmipp4
