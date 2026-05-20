// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/execution_context.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/multidimensional/kernel_manager.hpp>
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
		: m_device_properties()
		, m_device(create_device(catalog, index, m_device_properties))
		, m_active_queue(m_device->create_command_queue())
		, m_kernel_manager(
			catalog.get_service_manager<multidimensional::kernel_manager>()
		)
	{
	}

	const hardware::device_properties& get_device_properties() const noexcept
	{
		return m_device_properties;
	}

	hardware::device& get_device() const noexcept
	{
		XMIPP4_ASSERT( m_device );
		return *m_device;
	}

	std::shared_ptr<hardware::command_queue>
	set_active_queue(std::shared_ptr<hardware::command_queue> queue) noexcept
	{
		std::swap(queue, m_active_queue);
		return queue;
	}

	const std::shared_ptr<hardware::command_queue>&
	get_active_queue() const noexcept
	{
		return m_active_queue;
	}

	std::shared_ptr<hardware::memory_allocator>
	set_active_allocator(
		std::shared_ptr<hardware::memory_allocator> allocator
	) noexcept
	{
		std::swap(allocator, m_active_allocator);
		return allocator;
	}

	const std::shared_ptr<hardware::memory_allocator>&
	get_active_allocator() const noexcept
	{
		return m_active_allocator;
	}

	const multidimensional::kernel_manager& get_kernel_manager() const noexcept
	{
		return m_kernel_manager;
	}

private:
	hardware::device_properties m_device_properties;
	std::shared_ptr<hardware::device> m_device;
	std::shared_ptr<hardware::command_queue> m_active_queue;
	std::shared_ptr<hardware::memory_allocator> m_active_allocator;
	std::reference_wrapper<const multidimensional::kernel_manager> m_kernel_manager;

	static std::shared_ptr<hardware::device> create_device(
		service_catalog &catalog, 
		const hardware::device_index &index,
		hardware::device_properties &properties
	)
	{
		const auto &dev_manager = 
			catalog.get_service_manager<hardware::device_manager>();

		if(!dev_manager.get_device_properties(index, properties))
		{
			throw std::invalid_argument(
				"Requested device index does not exist"
			);
		}

		auto device = dev_manager.create_device(index);
		XMIPP4_ASSERT( device );
		return device;
	}
};



execution_context::execution_context() noexcept = default;

execution_context::execution_context(
	service_catalog &catalog, 
	const hardware::device_index &index
)
	: m_implementation(std::make_unique<implementation>(catalog, index))
{
}

execution_context::execution_context(
	execution_context &&other
) noexcept = default;

execution_context::~execution_context() = default;

execution_context& 
execution_context::operator=(execution_context &&other) noexcept = default;

const hardware::device_properties& 
execution_context::get_device_properties() const
{
	return get_implementation().get_device_properties();
}

hardware::device& execution_context::get_device() const
{
	return get_implementation().get_device();
}

std::shared_ptr<hardware::command_queue>
execution_context::set_active_queue(
	std::shared_ptr<hardware::command_queue> queue
)
{
	return get_implementation().set_active_queue(std::move(queue));
}

const std::shared_ptr<hardware::command_queue>&
execution_context::get_active_queue() const
{
	return get_implementation().get_active_queue();
}

std::shared_ptr<hardware::memory_allocator>
execution_context::set_active_allocator(
	std::shared_ptr<hardware::memory_allocator> allocator
)
{
	return get_implementation().set_active_allocator(std::move(allocator));
}

const std::shared_ptr<hardware::memory_allocator>& 
execution_context::get_active_allocator() const
{
	return get_implementation().get_active_allocator();
}

const multidimensional::kernel_manager& 
execution_context::get_kernel_manager() const
{
	return get_implementation().get_kernel_manager();
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
