// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation_dispatcher.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class execution_context
{
public:

	XMIPP4_CORE_API
	const std::shared_ptr<hardware::device_context>& 
	get_device_context() const noexcept;

	XMIPP4_CORE_API
	const std::shared_ptr<hardware::command_queue>& 
	get_active_queue() const noexcept;

	XMIPP4_CORE_API
	const std::shared_ptr<multidimensional::operation_dispatcher> &
	get_dispatcher() const noexcept;

private:
	std::shared_ptr<hardware::device_context> m_device_context;
	std::shared_ptr<hardware::command_queue> m_active_queue;
	std::shared_ptr<multidimensional::operation_dispatcher> m_dispatcher;
};

} // namespace multidimensional
} // namespace xmipp4
