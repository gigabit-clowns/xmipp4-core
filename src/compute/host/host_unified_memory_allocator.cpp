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
 * @file host_unified_memory_allocator.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_unified_memory_allocator.hpp
 * @date 2024-11-06
 * 
 */

#include <xmipp4/core/compute/host/host_unified_memory_allocator.hpp>

#include "default_host_unified_buffer.hpp"
#include <xmipp4/core/compute/host/host_device_queue.hpp>

namespace xmipp4
{
namespace compute
{

std::unique_ptr<device_buffer> 
host_unified_memory_allocator::create_device_buffer(numerical_type type,
                                                    std::size_t count,
                                                    device_queue& )
{
    return std::make_unique<default_host_unified_buffer>(type, count);
}

std::shared_ptr<device_buffer> 
host_unified_memory_allocator::create_device_buffer_shared(numerical_type type,
                                                           std::size_t count,
                                                           device_queue& )
{
    return std::make_shared<default_host_unified_buffer>(type, count);
}

std::unique_ptr<host_buffer> 
host_unified_memory_allocator::create_host_buffer(numerical_type type,
                                                  std::size_t count,
                                                  device_queue& )
{
    return std::make_unique<default_host_unified_buffer>(type, count);
}

std::shared_ptr<host_buffer> 
host_unified_memory_allocator::create_host_buffer_shared(numerical_type type,
                                                         std::size_t count,
                                                         device_queue& )
{
    return std::make_shared<default_host_unified_buffer>(type, count);
}

std::unique_ptr<host_buffer> 
host_unified_memory_allocator::create_host_buffer(numerical_type type, 
                                                  std::size_t count )
{
    return std::make_unique<default_host_unified_buffer>(type, count);
}

std::shared_ptr<host_buffer> 
host_unified_memory_allocator::create_host_buffer_shared(numerical_type type, 
                                                         std::size_t count )
{
    return std::make_shared<default_host_unified_buffer>(type, count);
}

} // namespace compute
} // namespace xmipp4
