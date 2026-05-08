// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/memory_resource.hpp>

#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/platform/constexpr.hpp>

#include "host_memory/host_memory_resource.hpp"

namespace xmipp4
{
namespace hardware
{

memory_resource::memory_resource() noexcept = default;
memory_resource::~memory_resource() = default;

memory_resource& get_host_memory_resource() noexcept
{
	return host_memory_resource::get();
}

bool is_device_accessible(
	const memory_resource &resource, 
	device &device
) noexcept
{
	if (!is_device_accessible(resource.get_kind()))
	{
		return false;
	}

	if (resource.get_target_device() == &device)
	{
		return true;
	}

	// Add more heuristics

	return false;
}

} // namespace hardware
} // namespace xmipp4
