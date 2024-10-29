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
 * @file host_device_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_device_backend.hpp
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/compute/host/host_device_backend.hpp>

#include <xmipp4/core/compute/host/host_device.hpp>
#include <xmipp4/core/core_version.hpp>
#include <xmipp4/core/system/host.hpp>

namespace xmipp4
{
namespace compute
{

static const std::string name = "host";

const std::string& host_device_backend::get_name() const noexcept
{
    return name;
}

version host_device_backend::get_version() const noexcept
{
    return get_core_version();
}

bool host_device_backend::is_available() const noexcept
{
    return true;
}

void host_device_backend::enumerate_devices(std::vector<std::size_t> &ids) const
{
    ids = { 0 };
}

bool host_device_backend::get_device_properties(std::size_t id, 
                                                device_properties &desc ) const
{
    bool result = false;

    if (id == 0)
    {
        desc.set_name(system::get_hostname());
        desc.set_type(device_type::cpu);
        desc.set_physical_location("");
        desc.set_total_memory_bytes(system::get_total_system_memory());
        result = true;
    }

    return result;
}

std::unique_ptr<device> 
host_device_backend::create_device(std::size_t id)
{
    std::unique_ptr<device> result;
    
    if (id == 0)
    {
        result = std::make_unique<host_device>();
    }

    return result;
}

std::shared_ptr<device> 
host_device_backend::create_device_shared(std::size_t id)
{
    std::shared_ptr<device> result;

    if (id == 0)
    {
        result = std::make_shared<host_device>();
    }

    return result;
}

bool host_device_backend::register_at(device_manager &manager)
{
    return manager.register_backend(std::make_unique<host_device_backend>());
}

} // namespace system
} // namespace xmipp4
