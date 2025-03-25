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

host_device::host_device()
    : m_queue_pool()
    , m_allocator(std::make_shared<host_unified_memory_allocator>())
    , m_transfer(std::make_shared<host_transfer>())
    , m_event(std::make_shared<host_event>())
{
}

host_device_queue_pool& host_device::get_queue_pool()
{
    return m_queue_pool;
}

std::shared_ptr<device_memory_allocator> 
host_device::create_device_memory_allocator() 
{
    return m_allocator;
}

std::shared_ptr<host_memory_allocator> 
host_device::create_host_memory_allocator() 
{
    return m_allocator;
}

std::shared_ptr<host_to_device_transfer> 
host_device::create_host_to_device_transfer()
{
    return m_transfer;
}

std::shared_ptr<device_to_host_transfer> 
host_device::create_device_to_host_transfer()
{
    return m_transfer;
}

std::shared_ptr<device_copy> 
host_device::create_device_copy()
{
    return m_transfer;
}

std::shared_ptr<device_event> host_device::create_device_event()
{
    return m_event;
}

std::shared_ptr<device_to_host_event> 
host_device::create_device_to_host_event()
{
    return m_event;
}

} // namespace compute
} // namespace xmipp4
