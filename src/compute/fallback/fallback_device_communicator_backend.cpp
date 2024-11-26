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
 * @date 2024-11-26
 * 
 */

#include <xmipp4/core/compute/fallback/fallback_device_communicator_backend.hpp>

#include <xmipp4/core/core_version.hpp>
#include <xmipp4/core/compute/device.hpp>
#include <xmipp4/core/communication/communicator.hpp>
#include <xmipp4/core/compute/fallback/fallback_device_communicator.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace compute
{

static std::size_t require_same_size(std::size_t size1, std::size_t size2)
{
    if (size1 != size2)
    {
        throw std::invalid_argument(
            "Result array must have the same size of the device array"
        );
    }

    return size1;
}



std::string fallback_device_communicator_backend::get_name() const noexcept
{
    return "fallback";
}

version fallback_device_communicator_backend::get_version() const noexcept
{
    return get_core_version();
}

bool fallback_device_communicator_backend::is_available() const noexcept
{
    return true;
}

backend_priority fallback_device_communicator_backend::get_priority() const noexcept
{
    return backend_priority::fallback;
}

bool fallback_device_communicator_backend
::supports_devices(span<device*>) const noexcept
{
    return true;
}

void fallback_device_communicator_backend
::create_device_communicators(node_communicator &node_communicator,
                              span<device*> devices,
                              span<std::unique_ptr<device_communicator>> result ) const
{
    const auto count = require_same_size(devices.size(), result.size());
    auto node_communicator_copy = node_communicator.clone_shared();
    for(std::size_t i = 0; i < count; ++i)
    {
        result[i] = std::make_unique<fallback_device_communicator>(
            node_communicator_copy,
            *(devices[i])
        );
    }
}

void fallback_device_communicator_backend
::create_device_communicators_shared(node_communicator &node_communicator,
                                     span<device*> devices,
                                     span<std::shared_ptr<device_communicator>> result ) const
{
    const auto count = require_same_size(devices.size(), result.size());
    auto node_communicator_copy = node_communicator.clone_shared();
    for(std::size_t i = 0; i < count; ++i)
    {
        result[i] = std::make_shared<fallback_device_communicator>(
            node_communicator_copy,
            *(devices[i])
        );
    }
}

} // namespace compute
} // namespace xmipp4
