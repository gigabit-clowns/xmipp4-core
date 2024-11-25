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
 * @file fallback_device_communicator_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of fallback_device_communicator_backend.hpp
 * @date 2024-11-25
 * 
 */

#include <xmipp4/core/compute/fallback/fallback_device_communicator_backend.hpp>

#include <xmipp4/core/compute/fallback/fallback_device_communicator.hpp>

namespace xmipp4
{
namespace compute
{


bool 
fallback_device_communicator_backend
::supports_devices(span<device*>) const noexcept
{
    return true;
}

void 
fallback_device_communicator_backend
::create_device_communicators(
    const std::shared_ptr<communication::communicator> &node_communicator,
    span<device*> devices,
    std::vector<std::unique_ptr<device_communicator>> &result
) const
{
    // TODO
}
    
void 
fallback_device_communicator_backend
::create_device_communicators_shared(
    const std::shared_ptr<communication::communicator> &node_communicator,
    span<device*> devices,
    std::vector<std::shared_ptr<device_communicator>> &result 
) const
{

}

} // namespace compute
} // namespace xmipp4
