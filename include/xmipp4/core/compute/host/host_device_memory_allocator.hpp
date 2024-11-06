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
 * @file host_device_memory_allocator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::host_device_memory_allocator class
 * @date 2024-11-06
 * 
 */


#include "../device_memory_allocator.hpp"
#include "../host_memory_allocator.hpp"


namespace xmipp4 
{
namespace compute
{

class host_device_memory_allocator
    : public device_memory_allocator
    , public host_memory_allocator
{
public:
    host_device_memory_allocator() = default;
    host_device_memory_allocator(const host_device_memory_allocator &other) = default;
    host_device_memory_allocator(host_device_memory_allocator &&other) = default;
    virtual ~host_device_memory_allocator() = default;

    host_device_memory_allocator& 
    operator=(const host_device_memory_allocator &other) = default;
    host_device_memory_allocator& 
    operator=(host_device_memory_allocator &&other) = default;

    std::unique_ptr<device_buffer> 
    create_buffer(numerical_type type, 
                  std::size_t count, 
                  device_queue &queue ) final;

    std::shared_ptr<device_buffer> 
    create_buffer_shared(numerical_type type, 
                         std::size_t count, 
                         device_queue &queue ) final;

    std::unique_ptr<host_buffer> 
    create_buffer(numerical_type type, std::size_t count ) final;

    std::shared_ptr<host_buffer> 
    create_buffer_shared(numerical_type type, 
                         std::size_t count ) final;

}; 

} // namespace compute
} // namespace xmipp4
