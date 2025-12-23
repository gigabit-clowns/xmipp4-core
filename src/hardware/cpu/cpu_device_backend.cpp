// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/cpu/cpu_device_backend.hpp>

#include <xmipp4/core/hardware/cpu/cpu_device.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/core_version.hpp>
#include <xmipp4/core/system/host.hpp>

namespace xmipp4
{
namespace hardware
{

std::string cpu_device_backend::get_name() const
{
	return "cpu";
}

version cpu_device_backend::get_version() const
{
	return get_core_version();
}

void cpu_device_backend::enumerate_devices(std::vector<std::size_t> &ids) const
{
	ids = { 0 };
}

bool cpu_device_backend::get_device_properties(
	std::size_t id, 
	device_properties &desc
) const
{
	bool result = false;

	if (id == 0)
	{
		desc.set_name(system::get_hostname());
		desc.set_type(device_type::cpu);
		desc.set_physical_location("");
		desc.set_total_memory_bytes(system::get_total_system_memory());
		desc.set_optimal_data_alignment(64); //AVX-512
		result = true;
	}

	return result;
}

std::shared_ptr<device> 
cpu_device_backend::create_device(std::size_t id)
{
	if (id != 0)
	{
		throw std::invalid_argument("Requested device id is invalid");
	}

	return std::make_shared<cpu_device>();
}

bool cpu_device_backend::register_at(device_manager &manager)
{
	return manager.register_backend(std::make_unique<cpu_device_backend>());
}

} // namespace hardware
} // namespace xmipp4
