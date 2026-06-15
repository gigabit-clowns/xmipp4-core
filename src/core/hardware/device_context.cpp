// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/device_context.hpp>

#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include "memory_allocator_table.hpp"

#include <stdexcept>
#include <utility>

namespace xmipp4
{
namespace hardware
{

class device_context::implementation
{
public:
	explicit implementation(std::shared_ptr<device> dev)
		: m_device(std::move(dev))
		, m_allocators(*m_device)
	{
	}

	const std::shared_ptr<device>& get_device() const noexcept
	{
		XMIPP4_ASSERT(m_device);
		return m_device;
	}

	memory_allocator_table& get_allocator_table() noexcept
	{
		return m_allocators;
	}

	const memory_allocator_table& get_allocator_table() const noexcept
	{
		return m_allocators;
	}

private:
	std::shared_ptr<device> m_device;
	memory_allocator_table m_allocators;
};



device_context::device_context() noexcept = default;

device_context::device_context(std::shared_ptr<device> dev)
{
	if (!dev)
	{
		throw std::invalid_argument(
			"Cannot construct a device_context from a null device."
		);
	}

	m_implementation = std::make_unique<implementation>(std::move(dev));
}

device_context::device_context(device_context &&other) noexcept = default;

device_context::~device_context() = default;

device_context&
device_context::operator=(device_context &&other) noexcept = default;



bool device_context::is_empty() const noexcept
{
	return m_implementation == nullptr;
}

const std::shared_ptr<device>& device_context::get_device() const noexcept
{
	XMIPP4_ASSERT(m_implementation);
	return m_implementation->get_device();
}

const memory_resource&
device_context::get_memory_resource(memory_resource_affinity affinity) const
{
	XMIPP4_ASSERT(m_implementation);
	return m_implementation->get_device()->get_memory_resource(affinity);
}

const device_properties& device_context::get_properties() const noexcept
{
	XMIPP4_ASSERT(m_implementation);
	return m_implementation->get_device()->get_properties();
}

const std::shared_ptr<memory_allocator>&
device_context::get_allocator(memory_resource_affinity affinity) const noexcept
{
	XMIPP4_ASSERT(m_implementation);
	return m_implementation->get_allocator_table().get_allocator(affinity);
}

std::shared_ptr<memory_allocator>
device_context::set_allocator(
	memory_resource_affinity affinity,
	std::shared_ptr<memory_allocator> allocator
) noexcept
{
	XMIPP4_ASSERT(m_implementation);
	return m_implementation->get_allocator_table().set_allocator(
		affinity, 
		std::move(allocator)
	);
}

std::shared_ptr<command_queue> device_context::get_default_queue() const
{
	XMIPP4_ASSERT(m_implementation);
	return m_implementation->get_device()->get_default_queue();
}

std::shared_ptr<command_queue> device_context::create_command_queue() const
{
	XMIPP4_ASSERT(m_implementation);
	return m_implementation->get_device()->create_command_queue();
}

std::shared_ptr<event>
device_context::create_event(event_usage_flags usage) const
{
	XMIPP4_ASSERT(m_implementation);
	return m_implementation->get_device()->create_event(usage);
}

} // namespace hardware
} // namespace xmipp4
