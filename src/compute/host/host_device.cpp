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
#include <xmipp4/core/compute/host/host_unified_memory_allocator.hpp>
#include <xmipp4/core/compute/host/host_transfer.hpp>
#include <xmipp4/core/compute/host/host_event.hpp>

namespace xmipp4
{
namespace compute
{

host_device_queue_pool& host_device::get_queue_pool()
{
    return m_queue_pool;
}

std::unique_ptr<device_memory_allocator> 
host_device::create_device_memory_allocator()
{
    return std::make_unique<host_unified_memory_allocator>();
}

std::shared_ptr<device_memory_allocator> 
host_device::create_device_memory_allocator_shared() 
{
    return std::make_shared<host_unified_memory_allocator>();
}
std::unique_ptr<host_memory_allocator> 
host_device::create_host_memory_allocator()
{
    return std::make_unique<host_unified_memory_allocator>();
}

std::shared_ptr<host_memory_allocator> 
host_device::create_host_memory_allocator_shared() 
{
    return std::make_shared<host_unified_memory_allocator>();
}

std::unique_ptr<host_to_device_transfer> 
host_device::create_host_to_device_transfer()
{
    return std::make_unique<host_transfer>();
}

std::shared_ptr<host_to_device_transfer> 
host_device::create_host_to_device_transfer_shared()
{
    return std::make_shared<host_transfer>();
}

std::unique_ptr<device_to_host_transfer> 
host_device::create_device_to_host_transfer()
{
    return std::make_unique<host_transfer>();
}

std::shared_ptr<device_to_host_transfer> 
host_device::create_device_to_host_transfer_shared()
{
    return std::make_shared<host_transfer>();
}

std::unique_ptr<device_copy> 
host_device::create_device_copy()
{
    return std::make_unique<host_transfer>();
}

std::shared_ptr<device_copy> 
host_device::create_device_copy_shared()
{
    return std::make_shared<host_transfer>();
}

std::unique_ptr<device_event> host_device::create_device_event()
{
    return std::make_unique<host_event>();
}

std::shared_ptr<device_event> host_device::create_device_event_shared()
{
    return std::make_shared<host_event>();
}

std::unique_ptr<device_to_host_event> 
host_device::create_device_to_host_event()
{
    return std::make_unique<host_event>();
}

std::shared_ptr<device_to_host_event> 
host_device::create_device_to_host_event_shared()
{
    return std::make_shared<host_event>();
}

} // namespace compute
} // namespace xmipp4
