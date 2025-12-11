// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/device_context.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/memory_allocator_manager.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

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
		m_device = dev_manager.create_device(index);

		XMIPP4_ASSERT( m_device );
		auto &device_optimal_memory_resource = 
			m_device->get_device_local_memory_resource();
		auto &host_accessible_memory_resource = 
			m_device->get_host_accessible_memory_resource();

		
		const auto &alloc_manager = 
			catalog.get_service_manager<memory_allocator_manager>();
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
	}

	device& get_device() const noexcept
	{
		XMIPP4_ASSERT( m_device );
		return *m_device;
	}

	memory_allocator& get_memory_allocator(target_placement placement) const
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

	memory_allocator& get_device_optimal_memory_allocator() const noexcept
	{
		XMIPP4_ASSERT( m_device_optimal_memory_allocator );
		return *m_device_optimal_memory_allocator;
	}

	memory_allocator& get_host_accessible_memory_allocator() const noexcept
	{
		XMIPP4_ASSERT( m_host_accessible_memory_allocator );
		return *m_host_accessible_memory_allocator;
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
	std::shared_ptr<device_queue> m_active_queue;
	std::shared_ptr<memory_allocator> m_device_optimal_memory_allocator;
	std::shared_ptr<memory_allocator> m_host_accessible_memory_allocator;
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
device_context::get_memory_allocator(target_placement placement) const
{
	return get_implementation().get_memory_allocator(placement);
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
