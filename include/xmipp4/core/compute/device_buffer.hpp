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
 * @file device_buffer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device_buffer interface
 * @date 2024-10-25
 * 
 */

#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class host_buffer;
class device_queue;

/**
 * @brief Abstract class defining an in-device memory
 * allocation.
 * 
 */
class XMIPP4_CORE_API device_buffer
{
public:
    device_buffer() = default;
    device_buffer(const device_buffer &other) = default;
    device_buffer(device_buffer &&other) = default;
    virtual ~device_buffer() = default;

    device_buffer& operator=(const device_buffer &other) = default;
    device_buffer& operator=(device_buffer &&other) = default;
    
    /**
     * @brief Get the number of bytes in the buffer.
     * 
     * @return std::size_t The size of the buffer.
     * 
     */
    virtual std::size_t get_size() const noexcept = 0;

    /**
     * @brief Get a host accessible alias of this buffer.
     * 
     * If this buffer is not host accessible, this method returns null.
     * 
     * @return host_buffer* Host accessible alias of this buffer.
     */
    virtual host_buffer* get_host_accessible_alias() noexcept = 0;

    /**
     * @brief Get a host accessible alias of this buffer.
     * 
     * If this buffer is not host accessible, this method returns null.
     * 
     * @return const host_buffer* Host accessible alias of this buffer.
     */
    virtual const host_buffer* get_host_accessible_alias() const noexcept = 0;

    /**
     * @brief Acknowledge that the buffer is being used in a queue other than
     * the one used for allocation.
     * 
     * This will prevent premature deallocation before the provided queue
     * has completed execution until the point where it can be safely 
     * deallocated.
     * 
     * @param queue The queue where the buffer is being used.
     * 
     */
    virtual void record_queue(device_queue &queue) = 0;

};



/**
 * @brief Get a host accessible alias of a device_buffer.
 * 
 * If this buffer is not host accessible, this method returns null.
 * 
 * @param buffer Buffer to be aliased. May be null, in which case null is
 * returned.
 * @return std::shared<host_buffer> Host accessible alias of the
 * provided buffer.
 */
std::shared_ptr<host_buffer> 
get_host_accessible_alias(const std::shared_ptr<device_buffer> &buffer) noexcept;

/**
 * @brief Get a host accessible alias of a device_buffer.
 * 
 * If this buffer is not host accessible, this method returns null.
 * 
 * @param buffer Buffer to be aliased. May be null, in which case null is
 * returned.
 * @return std::shared<const host_buffer> Host accessible alias of the
 * provided buffer.
 */
std::shared_ptr<const host_buffer> 
get_host_accessible_alias(const std::shared_ptr<const device_buffer> &buffer) noexcept;

} // namespace compute
} // namespace xmipp4
