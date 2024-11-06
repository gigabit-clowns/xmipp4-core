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
 * @file host_device.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_device.hpp
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/compute/host/host_device.hpp>

#include <xmipp4/core/compute/host/host_device_queue.hpp>
#include <xmipp4/core/compute/host/host_device_memory_allocator.hpp>
#include <xmipp4/core/compute/host/host_device_host_communicator.hpp>

namespace xmipp4
{
namespace compute
{

std::unique_ptr<device_queue> host_device::create_queue()
{
    return std::make_unique<host_device_queue>();
}

std::shared_ptr<device_queue> host_device::create_queue_shared()
{
    return std::make_shared<host_device_queue>();
}

std::unique_ptr<device_memory_allocator> 
host_device::create_device_memory_allocator()
{
    return std::make_unique<host_device_memory_allocator>();
}

std::shared_ptr<device_memory_allocator> 
host_device::create_device_memory_allocator_shared() 
{
    return std::make_shared<host_device_memory_allocator>();
}
std::unique_ptr<host_memory_allocator> 
host_device::create_host_memory_allocator()
{
    return std::make_unique<host_device_memory_allocator>();
}

std::shared_ptr<host_memory_allocator> 
host_device::create_host_memory_allocator_shared() 
{
    return std::make_shared<host_device_memory_allocator>();
}

std::unique_ptr<device_host_communicator> 
host_device::create_device_host_communicator()
{
    return std::make_unique<host_device_host_communicator>();
}

std::shared_ptr<device_host_communicator> 
host_device::create_device_host_communicator_shared()
{
    return std::make_shared<host_device_host_communicator>();
}

} // namespace compute
} // namespace xmipp4
