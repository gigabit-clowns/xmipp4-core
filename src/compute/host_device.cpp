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

#include <xmipp4/core/compute/host_device.hpp>

#include <xmipp4/core/compute/host_device_backend.hpp>

namespace xmipp4
{
namespace compute
{

host_device::host_device(host_device_backend &backend)
    : m_backend(backend)
{
}

device_backend& host_device::get_backend() const noexcept
{
    return m_backend;
}

std::unique_ptr<queue> host_device::create_queue()
{
    return nullptr; // TODO
}

std::shared_ptr<queue> host_device::create_queue_shared()
{
    return nullptr; // TODO
}

std::unique_ptr<device_buffer> 
host_device::create_buffer(numerical_type type,
                           std::size_t count )
{
    return nullptr; // TODO
}

std::shared_ptr<device_buffer> 
host_device::create_buffer_shared(numerical_type type,
                                  std::size_t count) 
{
    return nullptr; // TODO
}

} // namespace system
} // namespace xmipp4
