// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/execution_context.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/memory_allocator_manager.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include <stdexcept>

namespace xmipp4
{

class execution_context::implementation
{
public:
	implementation(
		service_catalog &catalog, 
		const hardware::device_index &index
	)
	{
		const auto &dev_manager = 
			catalog.get_service_manager<hardware::device_manager>();

		hardware::device_properties properties;
		if(!dev_manager.get_device_properties(index, properties))
		{
			throw std::invalid_argument(
				"Requested device index does not exist"
			);
		}

		m_device = dev_manager.create_device(index);

		XMIPP4_ASSERT( m_device );
		auto &device_optimal_memory_resource = 
			m_device->get_device_local_memory_resource();
		auto &host_accessible_memory_resource = 
			m_device->get_host_accessible_memory_resource();
		
		const auto &alloc_manager = 
			catalog.get_service_manager<hardware::memory_allocator_manager>();
		m_device_optimal_memory_allocator = 
			alloc_manager.create_memory_allocator(
				device_optimal_memory_resource
			);

		if (&device_optimal_memory_resource == &host_accessible_memory_resource)
		{
			m_host_accessible_memory_allocator = 
				m_device_optimal_memory_allocator;
		}
		else
		{
			m_host_accessible_memory_allocator = 
				alloc_manager.create_memory_allocator(
					host_accessible_memory_resource
				);
		}

		m_optimal_data_alignment = properties.get_optimal_data_alignment();
		// TODO operation dispatcher
	}

	hardware::device& get_device() const noexcept
	{
		XMIPP4_ASSERT( m_device );
		return *m_device;
	}

	hardware::memory_allocator& get_memory_allocator(target_placement placement) const
	{
		switch (placement)
		{
		case target_placement::device_optimal:
			return get_device_optimal_memory_allocator();
		case target_placement::host_accessible:
			return get_host_accessible_memory_allocator();
		default:
			throw std::invalid_argument("Invalid placement was provided");
		}
	}

	hardware::memory_allocator& get_device_optimal_memory_allocator() const noexcept
	{
		XMIPP4_ASSERT( m_device_optimal_memory_allocator );
		return *m_device_optimal_memory_allocator;
	}

	hardware::memory_allocator& get_host_accessible_memory_allocator() const noexcept
	{
		XMIPP4_ASSERT( m_host_accessible_memory_allocator );
		return *m_host_accessible_memory_allocator;
	}

	std::size_t get_optimal_data_alignment() const noexcept
	{
		return m_optimal_data_alignment;
	}

	std::shared_ptr<hardware::device_queue> 
	set_active_queue(std::shared_ptr<hardware::device_queue> queue) noexcept
	{
		std::swap(queue, m_active_queue);
		return queue;
	}

	const std::shared_ptr<hardware::device_queue>& 
	get_active_queue() const noexcept
	{
		return m_active_queue;
	}

	multidimensional::operation_dispatcher& 
	get_operation_dispatcher() const noexcept
	{
		XMIPP4_ASSERT(m_dispatcher);
		return *m_dispatcher;
	}

private:
	std::shared_ptr<hardware::device> m_device;
	std::shared_ptr<hardware::device_queue> m_active_queue;
	std::shared_ptr<hardware::memory_allocator> m_device_optimal_memory_allocator;
	std::shared_ptr<hardware::memory_allocator> m_host_accessible_memory_allocator;
	std::shared_ptr<multidimensional::operation_dispatcher> m_dispatcher;
	std::size_t m_optimal_data_alignment;
};



execution_context::execution_context() noexcept = default;

execution_context::execution_context(
	service_catalog &catalog, 
	const hardware::device_index &index
)
	: m_implementation(std::make_unique<implementation>(catalog, index))
{
}

execution_context::execution_context(execution_context &&other) noexcept = default;

execution_context::~execution_context() = default;

execution_context& 
execution_context::operator=(execution_context &&other) noexcept = default;

hardware::device& execution_context::get_device() const
{
	return get_implementation().get_device();
}

hardware::memory_allocator& 
execution_context::get_memory_allocator(target_placement placement) const
{
	return get_implementation().get_memory_allocator(placement);
}

std::size_t execution_context::get_optimal_data_alignment() const
{
	return get_implementation().get_optimal_data_alignment();
}

std::shared_ptr<hardware::device_queue> 
execution_context::set_active_queue(
	std::shared_ptr<hardware::device_queue> queue
)
{
	return get_implementation().set_active_queue(std::move(queue));
}

const std::shared_ptr<hardware::device_queue>& 
execution_context::get_active_queue() const
{
	return get_implementation().get_active_queue();
}

multidimensional::operation_dispatcher& 
execution_context::get_operation_dispatcher() const
{
	return get_implementation().get_operation_dispatcher();
}

execution_context::implementation& execution_context::get_implementation()
{
	if (!m_implementation)
	{
		throw invalid_operation_error(
			"Can not use a moved or default constructed execution_context"
		);
	}

	return *m_implementation;
}

const execution_context::implementation& 
execution_context::get_implementation() const
{
	if (!m_implementation)
	{
		throw invalid_operation_error(
			"Can not use a moved or default constructed execution_context"
		);
	}

	return *m_implementation;
}

} // namespace xmipp4
