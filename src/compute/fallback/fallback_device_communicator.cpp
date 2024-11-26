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
 * @file fallback_device_communicator.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of fallback_device_communicator.hpp
 * @date 2024-11-26
 * 
 */

#include <xmipp4/core/compute/fallback/fallback_device_communicator.hpp>

#include <xmipp4/core/compute/device.hpp>
#include <xmipp4/core/communication/communicator.hpp>

namespace xmipp4
{
namespace compute
{

fallback_device_communicator
::fallback_device_communicator(std::shared_ptr<node_communicator> node_communicator,
                               device& device ) noexcept
    : m_node_communicator(std::move(node_communicator))
    , m_device(device)
{
}

std::size_t fallback_device_communicator::get_size() const noexcept
{
    return m_node_communicator->get_size();
}

int fallback_device_communicator::get_rank() const noexcept
{
    return m_node_communicator->get_rank();
}

std::unique_ptr<device_memory_allocator>
fallback_device_communicator::create_device_memory_allocator()
{
    return m_device.get().create_device_memory_allocator();
}

std::shared_ptr<device_memory_allocator>
fallback_device_communicator::create_device_memory_allocator_shared()
{
    return m_device.get().create_device_memory_allocator_shared();
}

} // namespace compute
} // namespace xmipp4
