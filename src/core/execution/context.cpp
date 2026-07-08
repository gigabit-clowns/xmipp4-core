// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/execution/context.hpp>

#include <memory>
#include <utility>

namespace xmipp4
{
namespace execution
{

context::context() noexcept = default;

context::context(
	hardware::device_context device_context,
	std::shared_ptr<dispatcher> dispatcher
)
	: m_device_context(std::move(device_context))
	, m_dispatcher(std::move(dispatcher))
{
}

context::context(const context &other) = default;

context::context(context &&other) noexcept
	= default;

context::~context() = default;

context&
context::operator=(const context &other) = default;

context&
context::operator=(context &&other) noexcept = default;

const hardware::device_context&
context::get_device_context() const noexcept
{
	return m_device_context;
}

const std::shared_ptr<dispatcher>&
context::get_dispatcher() const noexcept
{
	return m_dispatcher;
}

context context::with_device_context(
	hardware::device_context device_context
) const
{
	return context(
		std::move(device_context),
		m_dispatcher
	);
}

context context::with_dispatcher(
	std::shared_ptr<dispatcher> dispatcher
) const
{
	return context(
		m_device_context,
		std::move(dispatcher)
	);
}

} // namespace execution
} // namespace xmipp4
