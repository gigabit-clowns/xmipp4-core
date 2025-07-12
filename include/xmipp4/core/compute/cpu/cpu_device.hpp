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
 * @file cpu_device.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines cpu_device interface
 * @date 2024-10-29
 * 
 */

#include "../device.hpp"

#include "cpu_device_queue_pool.hpp"

namespace xmipp4 
{
namespace compute
{

class cpu_unified_memory_allocator;
class cpu_transfer;
class cpu_event;

/**
 * @brief Special implementation of the device interface to be able to use
 * the "host" as a device.
 * 
 */
class cpu_device final
    : public device
{
public:
    cpu_device();
    ~cpu_device() override = default;

    cpu_device_queue_pool& get_queue_pool() override;

    std::shared_ptr<device_memory_allocator> 
    create_device_memory_allocator() override;

    std::shared_ptr<host_memory_allocator> 
    create_host_memory_allocator() override;

    std::shared_ptr<host_to_device_transfer> 
    create_host_to_device_transfer() override;

    std::shared_ptr<device_to_host_transfer> 
    create_device_to_host_transfer() override;

    std::shared_ptr<device_copy> 
    create_device_copy() override;

    std::shared_ptr<device_event> create_device_event() override;

    std::shared_ptr<device_to_host_event> 
    create_device_to_host_event() override;

private:
    cpu_device_queue_pool m_queue_pool;
    std::shared_ptr<cpu_unified_memory_allocator> m_allocator;
    std::shared_ptr<cpu_transfer> m_transfer;
    std::shared_ptr<cpu_event> m_event;

}; 

} // namespace compute
} // namespace xmipp4
