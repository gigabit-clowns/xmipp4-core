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
 * @file host_unified_memory_allocator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::host_unified_memory_allocator class
 * @date 2024-11-06
 * 
 */


#include "../device_memory_allocator.hpp"
#include "../host_memory_allocator.hpp"


namespace xmipp4 
{
namespace compute
{

/**
 * @brief Special implementation of device_memory_allocator and
 * host_memory_allocator that allows to allocate memory in the host 
 * as if it were a device.
 * 
 */
class host_unified_memory_allocator
    : public device_memory_allocator
    , public host_memory_allocator
{
public:
    host_unified_memory_allocator() = default;
    host_unified_memory_allocator(const host_unified_memory_allocator &other) = default;
    host_unified_memory_allocator(host_unified_memory_allocator &&other) = default;
    ~host_unified_memory_allocator() override = default;

    host_unified_memory_allocator& 
    operator=(const host_unified_memory_allocator &other) = default;
    host_unified_memory_allocator& 
    operator=(host_unified_memory_allocator &&other) = default;

    std::shared_ptr<host_unified_buffer> 
    create_unified_buffer(std::size_t size, std::size_t alignment);

    std::shared_ptr<device_buffer> 
    create_device_buffer(std::size_t size, 
                         std::size_t alignment,
                         device_queue &queue ) override;

    std::shared_ptr<host_buffer> 
    create_host_buffer(std::size_t size, 
                       std::size_t alignment,
                       device_queue &queue ) override;

    std::shared_ptr<host_buffer> 
    create_host_buffer(std::size_t size, std::size_t alignment) override;

}; 

} // namespace compute
} // namespace xmipp4
