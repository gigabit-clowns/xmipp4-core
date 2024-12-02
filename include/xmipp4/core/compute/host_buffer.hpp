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
 * @file host_buffer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::host_buffer interface
 * @date 2024-11-06
 * 
 */

#include "copy_region.hpp"
#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class device_buffer;
class device_queue;

/**
 * @brief Abstract class defining an in-host memory
 * allocation.
 * 
 */
class XMIPP4_CORE_API host_buffer
{
public:
    host_buffer() = default;
    host_buffer(const host_buffer &other) = default;
    host_buffer(host_buffer &&other) = default;
    virtual ~host_buffer() = default;

    host_buffer& operator=(const host_buffer &other) = default;
    host_buffer& operator=(host_buffer &&other) = default;

    /**
     * @brief Get the number of bytes in the buffer.
     * 
     * @return std::size_t The size of the buffer.
     * 
     */
    virtual std::size_t get_size() const noexcept = 0;

    /**
     * @brief Get the data.
     * 
     * @return void* The data.
     */
    virtual void* get_data() noexcept = 0;

    /**
     * @brief Get the data.
     * 
     * @return const void* The data.
     */
    virtual const void* get_data() const noexcept = 0;

    /**
     * @brief Get a device accessible alias of this buffer.
     * 
     * If this buffer is not device accessible, this method returns null.
     * 
     * @return device_buffer* Device accessible alias of this buffer.
     * 
     */
    virtual device_buffer* get_device_accessible_alias() noexcept = 0;

    /**
     * @brief Get a device accessible alias of this buffer.
     * 
     * If this buffer is not device accessible, this method returns null.
     * 
     * @return const device_buffer* Device accessible alias of this buffer.
     */
    virtual 
    const device_buffer* get_device_accessible_alias() const noexcept = 0;

    /**
     * @brief Acknowledge that the buffer is being used in a queue other than
     * the one used for allocation.
     * 
     * @param queue The queue where the buffer is being used.
     * 
     */
    virtual void record_queue(device_queue &queue) = 0;

};


/**
 * @brief Copy the contents of one buffer to another.
 * 
 * @param src_buffer Buffer from which elements are copied.
 * @param dst_buffer Buffer to which elements are copied.
 * 
 * @note Both buffers must have the same size.
 * 
 */
XMIPP4_CORE_API
void copy(const host_buffer &src_buffer, host_buffer &dst_buffer);

/**
 * @brief Copy regions of one buffer to another.
 * 
 * @param src_buffer Buffer from which elements are copied.
 * @param dst_buffer Buffer to which elements are copied.
 * @param regions Regions to be copied.
 * 
 * @note All values in regions are expressed in terms of bytes.
 * @note For all regions, source_offset+count must not be greater
 * than the source buffer size and destination_offset+count
 * must not be greater than the destination buffer size.
 * @note Regions should not overlap within a domain.
 * 
 */
XMIPP4_CORE_API
void copy(const host_buffer &src_buffer, host_buffer &dst_buffer,
          span<const copy_region> regions );

} // namespace compute
} // namespace xmipp4
