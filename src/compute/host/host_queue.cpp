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
 * @file host_queue.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_queue.hpp
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/compute/host/host_queue.hpp>

#include <xmipp4/core/compute/host/host_device.hpp>

namespace xmipp4
{
namespace compute
{

host_queue::host_queue(host_device &device)
    : m_device(device)
{
}

device& host_queue::get_device() const
{
    return m_device;
}

void host_queue::synchronize() const
{
    // NO-OP
}

} // namespace system
} // namespace xmipp4
