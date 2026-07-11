// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_command_queue.hpp"

#include <xmipp4/cpu/hardware/cpu_program.hpp>
#include <xmipp4/core/hardware/command.hpp>

#include <stdexcept>

namespace xmipp4
{

std::shared_ptr<cpu_command_queue> cpu_command_queue::m_instance;

void cpu_command_queue::submit(const command &cmd)
{
	const auto& prog = cmd.get_program();
	if (!prog)
	{
		throw std::invalid_argument(
			"cpu_command_queue::submit: Provided command does not have an "
			"associated program."
		);
	}

	const auto& cpu_prog = dynamic_cast<const cpu_program&>(*prog);
	cpu_prog.execute(
		cmd.get_outputs(),
		cmd.get_inputs(),
		cmd.get_scratch()
	);
}

void cpu_command_queue::signal(event &/*event*/)
{
	// No-op, synchronous execution.
}

void cpu_command_queue::wait(const event&)
{
	// No-op, synchronous execution.
}

std::shared_ptr<cpu_command_queue> cpu_command_queue::create()
{
	// As cpu_command_queue lacks any state, we can use the singleton pattern
	if (m_instance == nullptr)
	{
		m_instance = std::make_shared<cpu_command_queue>();
	}

	XMIPP4_ASSERT(m_instance);
	return m_instance;
}

cpu_command_queue*
cpu_command_queue::try_cast(command_queue &queue) noexcept
{
	if (&queue == m_instance.get())
	{
		return m_instance.get();
	}

	return dynamic_cast<cpu_command_queue*>(&queue);
}

const cpu_command_queue*
cpu_command_queue::try_cast(const command_queue &queue) noexcept
{
	if (&queue == m_instance.get())
	{
		return m_instance.get();
	}

	return dynamic_cast<const cpu_command_queue*>(&queue);
}

} // namespace xmipp4
