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

#include "numerical_type.hpp"
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
     * @param type Numerical type of the buffer.
     * @param count Number of elements in the buffer.
     * @return std::unique_ptr<host_buffer> The buffer.
     * 
     */
    virtual std::unique_ptr<host_buffer> 
    create_buffer(numerical_type type, 
                  std::size_t count ) = 0;

    /**
     * @brief Allocate a buffer in this host.
     * 
     * @param type Numerical type of the buffer.
     * @param count Number of elements in the buffer.
     * @return std::shared_ptr<host_buffer> The buffer.
     * 
     */
    virtual std::shared_ptr<host_buffer> 
    create_buffer_shared(numerical_type type, 
                         std::size_t count ) = 0;

}; 

} // namespace compute
} // namespace xmipp4
