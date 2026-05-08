// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/execution_context.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/memory_allocator_manager.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/multidimensional/kernel_manager.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "hardware/memory_allocator_pool.hpp"

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
		, m_allocator_pool(
			*m_device, 
			catalog.get_service_manager<hardware::memory_allocator_manager>()
		)
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

	hardware::memory_allocator& get_memory_allocator(
		hardware::memory_resource_affinity affinity
	) const
	{
		return m_allocator_pool.get_memory_allocator(affinity);
	}

	std::shared_ptr<hardware::device_executor>
	set_active_executor(std::shared_ptr<hardware::device_executor> executor) noexcept
	{
		std::swap(executor, m_active_executor);
		return executor;
	}

	const std::shared_ptr<hardware::device_executor>&
	get_active_executor() const noexcept
	{
		return m_active_executor;
	}

	const multidimensional::kernel_manager& get_kernel_manager() const noexcept
	{
		return m_kernel_manager;
	}

private:
	hardware::device_properties m_device_properties;
	std::shared_ptr<hardware::device> m_device;
	hardware::memory_allocator_pool m_allocator_pool;
	std::shared_ptr<hardware::device_executor> m_active_executor;
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

hardware::memory_allocator& 
execution_context::get_memory_allocator(
	hardware::memory_resource_affinity affinity
) const
{
	return get_implementation().get_memory_allocator(affinity);
}

std::shared_ptr<hardware::device_executor>
execution_context::set_active_executor(
	std::shared_ptr<hardware::device_executor> executor
)
{
	return get_implementation().set_active_executor(std::move(executor));
}

const std::shared_ptr<hardware::device_executor>&
execution_context::get_active_executor() const
{
	return get_implementation().get_active_executor();
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
