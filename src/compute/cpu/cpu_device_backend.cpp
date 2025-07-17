// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file cpu_device_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of cpu_device_backend.hpp
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/compute/cpu/cpu_device_backend.hpp>

#include <xmipp4/core/compute/cpu/cpu_device.hpp>
#include <xmipp4/core/core_version.hpp>
#include <xmipp4/core/system/host.hpp>

namespace xmipp4
{
namespace compute
{

std::string cpu_device_backend::get_name() const noexcept
{
    return "cpu";
}

version cpu_device_backend::get_version() const noexcept
{
    return get_core_version();
}

bool cpu_device_backend::is_available() const noexcept
{
    return true;
}

backend_priority cpu_device_backend::get_priority() const noexcept
{
    return backend_priority::normal;
}

void cpu_device_backend::enumerate_devices(std::vector<std::size_t> &ids) const
{
    ids = { 0 };
}

bool cpu_device_backend::get_device_properties(std::size_t id, 
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

std::shared_ptr<device> 
cpu_device_backend::create_device(std::size_t id, 
                                   const device_create_parameters& )
{
    std::shared_ptr<device> result;

    if (id == 0)
    {
        result = std::make_shared<cpu_device>();
    }

    return result;
}

bool cpu_device_backend::register_at(device_manager &manager)
{
    return manager.register_backend(std::make_unique<cpu_device_backend>());
}

} // namespace compute
} // namespace xmipp4
