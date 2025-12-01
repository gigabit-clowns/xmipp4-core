// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/memory_resource.hpp>

#include <xmipp4/core/hardware/device.hpp>

#include "host_memory/host_memory_resource.hpp"

namespace xmipp4
{
namespace hardware
{

memory_resource& get_host_memory_resource() noexcept
{
	return host_memory_resource::get();
}

bool is_device_accessible(
	const memory_resource &resource, 
	device &device
) noexcept
{
	if (&device.get_device_local_memory_resource() == &resource)
	{
		return true;
	}

	const auto kind = resource.get_kind();
	const auto target = resource.get_target_device();
	if (&device == target && is_device_accessible(kind))
	{
		return true;
	}

	return false;
}

} // namespace hardware
} // namespace xmipp4
