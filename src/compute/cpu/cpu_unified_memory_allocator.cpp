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
 * @file cpu_unified_memory_allocator.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of cpu_unified_memory_allocator.hpp
 * @date 2024-11-06
 * 
 */

#include <xmipp4/core/compute/cpu/cpu_unified_memory_allocator.hpp>

#include "default_cpu_unified_buffer.hpp"
#include <xmipp4/core/compute/cpu/cpu_device_queue.hpp>

namespace xmipp4
{
namespace compute
{

std::shared_ptr<cpu_unified_buffer> 
cpu_unified_memory_allocator::create_unified_buffer(std::size_t size, 
                                                     std::size_t alignment )
{
    return std::make_shared<default_cpu_unified_buffer>(size, alignment);
}

std::shared_ptr<device_buffer> 
cpu_unified_memory_allocator::create_device_buffer(std::size_t size,
                                                    std::size_t alignment,
                                                    device_queue& )
{
    return create_unified_buffer(size, alignment);
}

std::shared_ptr<host_buffer> 
cpu_unified_memory_allocator::create_host_buffer(std::size_t size,
                                                  std::size_t alignment,
                                                  device_queue& )
{
    return create_unified_buffer(size, alignment);
}

std::shared_ptr<host_buffer> 
cpu_unified_memory_allocator::create_host_buffer(std::size_t size,
                                                  std::size_t alignment )
{
    return create_unified_buffer(size, alignment);
}

} // namespace compute
} // namespace xmipp4
