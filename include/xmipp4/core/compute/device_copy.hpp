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
 * @file device_copy.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device_copy interface
 * @date 2024-11-15
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
class device_memory_allocator;



/**
 * @brief Abstract representation of a host to device transfer engine.
 * 
 * This class enables transferring data from host to device.
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
    copy(const device_copy &src_buffer,
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
