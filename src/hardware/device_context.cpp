// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/device_context.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/memory_allocator_manager.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "memory_allocator_pool.hpp"

namespace xmipp4
{
namespace hardware
{

class device_context::implementation
{
public:
	implementation(
		service_catalog &catalog, 
		const device_index &index
	)
	{
		const auto &dev_manager = catalog.get_service_manager<device_manager>();

		device_properties properties;
		if(!dev_manager.get_device_properties(index, properties))
		{
			throw std::invalid_argument(
				"Requested device index does not exist"
			);
		}

		m_device = dev_manager.create_device(index);
		XMIPP4_ASSERT( m_device );

		m_allocator_pool = std::make_unique<memory_allocator_pool>(
			*m_device,
			catalog.get_service_manager<memory_allocator_manager>()
		);

		m_optimal_data_alignment = properties.get_optimal_data_alignment();
	}

	device& get_device() const noexcept
	{
		XMIPP4_ASSERT( m_device );
		return *m_device;
	}

	memory_allocator& get_memory_allocator(
		memory_resource_affinity affinity
	) const
	{
		XMIPP4_ASSERT(m_allocator_pool);
		return m_allocator_pool->get_memory_allocator(affinity);
	}

	std::size_t get_optimal_data_alignment() const noexcept
	{
		return m_optimal_data_alignment;
	}

	std::shared_ptr<device_queue> 
	set_active_queue(std::shared_ptr<device_queue> queue) noexcept
	{
		std::swap(queue, m_active_queue);
		return queue;
	}

	const std::shared_ptr<device_queue>& get_active_queue() const noexcept
	{
		return m_active_queue;
	}

private:
	std::shared_ptr<device> m_device;
	std::unique_ptr<memory_allocator_pool> m_allocator_pool;
	std::shared_ptr<device_queue> m_active_queue;
	std::size_t m_optimal_data_alignment;
};



device_context::device_context() noexcept = default;

device_context::device_context(
	service_catalog &catalog, 
	const device_index &index
)
	: m_implementation(std::make_unique<implementation>(catalog, index))
{
}

device_context::device_context(device_context &&other) noexcept = default;

device_context::~device_context() = default;

device_context& 
device_context::operator=(device_context &&other) noexcept = default;

device& device_context::get_device() const
{
	return get_implementation().get_device();
}

memory_allocator& 
device_context::get_memory_allocator(memory_resource_affinity affinity) const
{
	return get_implementation().get_memory_allocator(affinity);
}

std::size_t device_context::get_optimal_data_alignment() const
{
	return get_implementation().get_optimal_data_alignment();
}

std::shared_ptr<device_queue> 
device_context::set_active_queue(std::shared_ptr<device_queue> queue)
{
	return get_implementation().set_active_queue(std::move(queue));
}

const std::shared_ptr<device_queue>& device_context::get_active_queue() const
{
	return get_implementation().get_active_queue();
}

device_context::implementation& device_context::get_implementation()
{
	if (!m_implementation)
	{
		throw invalid_operation_error(
			"Can not use a moved or default constructed device_context"
		);
	}

	return *m_implementation;
}

const device_context::implementation& device_context::get_implementation() const
{
	if (!m_implementation)
	{
		throw invalid_operation_error(
			"Can not use a moved or default constructed device_context"
		);
	}

	return *m_implementation;
}

} // namespace hardware
} // namespace xmipp4
