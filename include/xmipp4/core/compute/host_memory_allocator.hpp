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
 * @file host_memory_allocator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::host_memory_allocator interface
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

class host_buffer;


/**
 * @brief Abstract class defining an in-host memory allocator.
 * 
 * @note Allocations and deallocations are thread safe.
 * 
 */
class XMIPP4_CORE_API host_memory_allocator
{
public:
    host_memory_allocator() = default;
    host_memory_allocator(const host_memory_allocator &other) = default;
    host_memory_allocator(host_memory_allocator &&other) = default;
    virtual ~host_memory_allocator() = default;

    host_memory_allocator& 
    operator=(const host_memory_allocator &other) = default;
    host_memory_allocator& 
    operator=(host_memory_allocator &&other) = default;

    /**
     * @brief Allocate a buffer in this host.
     * 
     * @param size Number of bytes in the buffer.
     * @param alignment Alignment requirements for the data in the buffer.
     * @param queue Queue where the allocation and deallocation takes place.
     * @return std::unique_ptr<host_buffer> The buffer.
     * 
     * @note Using the buffer in an queue other than the one
     * used for allocation-deallocation requires explicit
     * synchronization.
     * @note Memory allocated here is not available to the host
     * until the queue has been executed until this point. Use
     * device_to_host synchronization primitives to assure that
     * the buffer is accessible.
     */
    virtual std::unique_ptr<host_buffer> 
    create_host_buffer(std::size_t size,
                       std::size_t alignment,
                       device_queue& queue ) = 0;

    /**
     * @brief Allocate a buffer in this host.
     * 
     * @param size Number of bytes in the buffer.
     * @param alignment Alignment requirements for the data in the buffer.
     * @param queue Queue where the allocation and deallocation takes place.
     * @return std::shared_ptr<host_buffer> The buffer.
     * 
     * @note Using the buffer in an queue other than the one
     * used for allocation-deallocation requires explicit
     * synchronization.
     * @note Memory allocated here is not available to the host
     * until the queue has been executed until this point. Use
     * device_to_host synchronization primitives to assure that
     * the buffer is accessible.
     */
    virtual std::shared_ptr<host_buffer> 
    create_host_buffer_shared(std::size_t size, 
                              std::size_t alignment,
                              device_queue &queue ) = 0;

    /**
     * @brief Allocate a buffer in this host.
     * 
     * @param size Number of bytes in the buffer.
     * @param alignment Alignment requirements for the data in the buffer.
     * @return std::unique_ptr<host_buffer> The buffer.
     * 
     * @note Unlike the previous functions, the memory allocated here
     * is immediately available to the host. Likewise, the memory is
     * immediately deallocated upon buffer destruction. If used on a queue
     * remember recording the queue onto the buffer to prevent premature
     * deallocation.
     */
    virtual std::unique_ptr<host_buffer> 
    create_host_buffer(std::size_t size, std::size_t alignment) = 0;

    /**
     * @brief Allocate a buffer in this host.
     * 
     * @param size Number of bytes in the buffer.
     * @param alignment Alignment requirements for the data in the buffer.
     * @return std::shared_ptr<host_buffer> The buffer.
     * 
     * @note Unlike the previous functions, the memory allocated here
     * is immediately available to the host. Likewise, the memory is
     * immediately deallocated upon buffer destruction. If used on a queue
     * remember recording the queue onto the buffer to prevent premature
     * deallocation.
     */
    virtual std::shared_ptr<host_buffer> 
    create_host_buffer_shared(std::size_t size, std::size_t alignment) = 0;

}; 

} // namespace compute
} // namespace xmipp4
