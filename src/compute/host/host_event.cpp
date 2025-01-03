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
 * @file host_event.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_event.hpp
 * @date 2024-11-13
 * 
 */

#include <xmipp4/core/compute/host/host_event.hpp>


namespace xmipp4
{
namespace compute
{

void host_event::signal(device_queue&)
{
    // No-op
}

void host_event::wait() const
{
    // No-op
}

void host_event::wait(device_queue&) const
{
    // No-op
}

bool host_event::is_signaled() const
{
    return true;
}

} // namespace compute
} // namespace xmipp4
