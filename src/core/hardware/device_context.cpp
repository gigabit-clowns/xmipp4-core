// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/device_context.hpp>

#include <xmipp4/core/hardware/device_instance.hpp>
#include <xmipp4/core/hardware/device.hpp>

#include <stdexcept>
#include <utility>

namespace xmipp4
{
namespace hardware
{

device_context::device_context(std::shared_ptr<const device_instance> instance)
	: m_instance(std::move(instance))
{
	if (!m_instance)
	{
		throw std::invalid_argument(
			"Cannot construct a device_context from a null device_instance."
		);
	}

	m_active_queue = m_instance->get_default_queue();

	for (std::size_t i = 0; i < m_allocators.size(); ++i)
	{
		const auto affinity = static_cast<memory_resource_affinity>(i);
		m_allocators[i] = m_instance->get_allocator(affinity);
	}
}

const std::shared_ptr<const device_instance>&
device_context::get_device_instance() const noexcept
{
	return m_instance;
}

const std::shared_ptr<command_queue>&
device_context::get_active_queue() const noexcept
{
	return m_active_queue;
}

const std::shared_ptr<memory_allocator>&
device_context::get_allocator(memory_resource_affinity affinity) const noexcept
{
	return m_allocators[static_cast<std::size_t>(affinity)];
}

device_context
device_context::on_queue(std::shared_ptr<command_queue> queue) const
{
	if (!queue && m_instance)
	{
		queue = m_instance->get_default_queue();
	}

	auto result = *this;
	result.m_active_queue = std::move(queue);
	return result;
}

device_context
device_context::with_allocator(
	memory_resource_affinity affinity,
	std::shared_ptr<memory_allocator> allocator
) const
{
	if (!allocator && m_instance)
	{
		allocator = m_instance->get_allocator(affinity);
	}

	auto result = *this;
	const auto index = static_cast<std::size_t>(affinity);
	result.m_allocators[index] = std::move(allocator);
	return result;
}

} // namespace hardware
} // namespace xmipp4
