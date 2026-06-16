// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/execution_context.hpp>

#include <memory>
#include <utility>

namespace xmipp4
{
namespace multidimensional
{

execution_context::execution_context() noexcept = default;

execution_context::execution_context(
	hardware::device_context device_context,
	std::shared_ptr<multidimensional::operation_dispatcher> dispatcher
)
	: m_device_context(std::move(device_context))
	, m_dispatcher(std::move(dispatcher))
{
}

execution_context::execution_context(const execution_context &other) = default;

execution_context::execution_context(execution_context &&other) noexcept
	= default;

execution_context::~execution_context() = default;

execution_context&
execution_context::operator=(const execution_context &other) = default;

execution_context&
execution_context::operator=(execution_context &&other) noexcept = default;

const hardware::device_context&
execution_context::get_device_context() const noexcept
{
	return m_device_context;
}

const std::shared_ptr<multidimensional::operation_dispatcher>&
execution_context::get_dispatcher() const noexcept
{
	return m_dispatcher;
}

execution_context
execution_context::on_device(
	std::shared_ptr<const hardware::device_instance> device_instance
) const
{
	return execution_context(
		hardware::device_context(std::move(device_instance)),
		m_dispatcher
	);
}

execution_context execution_context::on_queue(
	std::shared_ptr<hardware::command_queue> queue
) const
{
	return execution_context(
		m_device_context.on_queue(std::move(queue)),
		m_dispatcher
	);
}

execution_context execution_context::with_allocator(
	hardware::memory_resource_affinity affinity,
	std::shared_ptr<hardware::memory_allocator> allocator
) const
{
	return execution_context(
		m_device_context.with_allocator(affinity, std::move(allocator)),
		m_dispatcher
	);
}

execution_context execution_context::with_dispatcher(
	std::shared_ptr<multidimensional::operation_dispatcher> dispatcher
) const
{
	return execution_context(
		m_device_context,
		std::move(dispatcher)
	);
}

} // namespace multidimensional
} // namespace xmipp4
