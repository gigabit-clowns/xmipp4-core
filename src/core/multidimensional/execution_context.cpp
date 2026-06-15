// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/execution_context.hpp>

#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>

#include "eager_operation_dispatcher.hpp"

#include <core/config.hpp>

#include <memory>
#include <utility>

namespace xmipp4
{
namespace multidimensional
{

namespace
{

std::shared_ptr<hardware::command_queue>
default_queue_of(
	const std::shared_ptr<const hardware::device_context> &device_context
)
{
	return device_context ? device_context->get_default_queue() : nullptr;
}

} // anonymous namespace

execution_context::execution_context() noexcept = default;

execution_context::execution_context(
	std::shared_ptr<const hardware::device_context> device_context,
	std::shared_ptr<const operation_command_manager> command_manager
)
	: execution_context(
		std::move(device_context),
		std::make_shared<eager_operation_dispatcher>(
			std::move(command_manager),
			XMIPP4_DEFAULT_OPERATION_COMMAND_CACHE_CAPACITY
		)
	)
{
}

execution_context::execution_context(
	std::shared_ptr<const hardware::device_context> device_context,
	std::shared_ptr<multidimensional::operation_dispatcher> dispatcher
)
	: execution_context(
		device_context,
		default_queue_of(device_context),
		std::move(dispatcher)
	)
{
}

execution_context::execution_context(
	std::shared_ptr<const hardware::device_context> device_context,
	std::shared_ptr<hardware::command_queue> active_queue,
	std::shared_ptr<multidimensional::operation_dispatcher> dispatcher
) noexcept
	: m_device_context(std::move(device_context))
	, m_active_queue(std::move(active_queue))
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

const std::shared_ptr<const hardware::device_context>&
execution_context::get_device_context() const noexcept
{
	return m_device_context;
}

const std::shared_ptr<hardware::command_queue>&
execution_context::get_active_queue() const noexcept
{
	return m_active_queue;
}

const std::shared_ptr<multidimensional::operation_dispatcher>&
execution_context::get_dispatcher() const noexcept
{
	return m_dispatcher;
}

execution_context
execution_context::on_device(
	std::shared_ptr<const hardware::device_context> device_context
) const
{
	auto active_queue = default_queue_of(device_context);
	return execution_context(
		std::move(device_context),
		std::move(active_queue),
		m_dispatcher
	);
}

execution_context
execution_context::on_queue(
	std::shared_ptr<hardware::command_queue> queue
) const
{
	return execution_context(
		m_device_context,
		std::move(queue),
		m_dispatcher
	);
}

execution_context
execution_context::with_dispatcher(
	std::shared_ptr<multidimensional::operation_dispatcher> dispatcher
) const
{
	return execution_context(
		m_device_context,
		m_active_queue,
		std::move(dispatcher)
	);
}

} // namespace multidimensional
} // namespace xmipp4
