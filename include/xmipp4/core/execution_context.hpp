// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "hardware/command_queue.hpp"
#include "multidimensional/operation_dispatcher.hpp"

#include <memory>

namespace xmipp4 
{

class execution_context
{
public:

	const std::shared_ptr<hardware::command_queue>& 
	get_active_queue() const noexcept;

	const std::shared_ptr<multidimensional::operation_dispatcher> &
	get_dispatcher() const noexcept;

private:
	std::shared_ptr<void> m_device; // TODO
	std::shared_ptr<hardware::command_queue> m_active_queue;
	std::shared_ptr<multidimensional::operation_dispatcher> m_dispatcher;
};

} // namespace xmipp4
