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
 * @file device_memory_allocator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device_memory_allocator interface
 * @date 2024-10-31
 * 
 */

#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class device_queue;
class device_buffer;



/**
 * @brief Abstract class defining an in-device memory allocator.
 * 
 */
class XMIPP4_CORE_API device_memory_allocator
{
public:
    device_memory_allocator() = default;
    device_memory_allocator(const device_memory_allocator &other) = default;
    device_memory_allocator(device_memory_allocator &&other) = default;
    virtual ~device_memory_allocator() = default;

    device_memory_allocator& 
    operator=(const device_memory_allocator &other) = default;
    device_memory_allocator& 
    operator=(device_memory_allocator &&other) = default;

    /**
     * @brief Allocate a buffer in this device.
     * 
     * @param size Number of bytes in the buffer.
     * @param alignment Alignment requirements for the data in the buffer.
     * @param queue Queue where the allocation and deallocation takes place.
     * @return std::shared_ptr<device_buffer> The buffer.

     * @note Using the buffer in an queue other than the one
     * used for allocation-deallocation requires explicit
     * synchronization.
     */
    virtual std::shared_ptr<device_buffer> 
    create_device_buffer(std::size_t size, 
                         std::size_t alignment,
                         device_queue &queue ) = 0;

}; 

} // namespace compute
} // namespace xmipp4
