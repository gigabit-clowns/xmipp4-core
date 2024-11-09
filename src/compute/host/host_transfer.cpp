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
 * @file host_transfer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_transfer.hpp
 * @date 2024-11-06
 * 
 */

#include <xmipp4/core/compute/host/host_transfer.hpp>

#include <xmipp4/core/compute/host/host_unified_buffer.hpp>
#include <xmipp4/core/compute/numerical_type.hpp>

#include <stdexcept>
#include <cstring>

namespace xmipp4
{
namespace compute
{

void host_transfer::transfer_copy(const std::shared_ptr<const host_buffer> &src_buffer, 
                                  device_buffer &dst_buffer, 
                                  device_queue& )
{
    if (!src_buffer)
    {
        throw std::invalid_argument("src_buffer cannot be nullptr");
    }

    copy(*src_buffer, dynamic_cast<host_unified_buffer&>(dst_buffer));
}

std::shared_ptr<device_buffer> 
host_transfer::transfer(const std::shared_ptr<host_buffer> &buffer, 
                        device_memory_allocator&,
                        device_queue&)
{
    return std::dynamic_pointer_cast<device_buffer>(buffer); // Alias
}

std::shared_ptr<const device_buffer> 
host_transfer::transfer(const std::shared_ptr<const host_buffer> &buffer, 
                        device_memory_allocator&,
                        device_queue& )
{
    return std::dynamic_pointer_cast<const device_buffer>(buffer); // Alias
}

void host_transfer::transfer_copy(const device_buffer &src_buffer,
                                  const std::shared_ptr<host_buffer> &dst_buffer,
                                  device_queue& )
{
    if (!dst_buffer)
    {
        throw std::invalid_argument("dst_buffer cannot be nullptr");
    }

    copy(dynamic_cast<const host_unified_buffer&>(src_buffer), *dst_buffer);
}

std::shared_ptr<host_buffer> 
host_transfer::transfer(const std::shared_ptr<device_buffer> &buffer, 
                        host_memory_allocator&,
                        device_queue& )
{
    return std::dynamic_pointer_cast<host_buffer>(buffer); // Alias
}

std::shared_ptr<const host_buffer> 
host_transfer::transfer(const std::shared_ptr<const device_buffer> &buffer, 
                        host_memory_allocator&,
                        device_queue& )
{
    return std::dynamic_pointer_cast<const host_buffer>(buffer); // Alias
}

void host_transfer::wait()
{
    // No-op, synchronous transfer.
}

void host_transfer::wait(device_queue&)
{
    // No-op, synchronous transfer.
}

} // namespace compute
} // namespace xmipp4
