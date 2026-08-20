// SPDX-License-Identifier: GPL-3.0-only

#include "command_queue.hpp"

#include <xmipp4/backends/cpu/program.hpp>
#include <xmipp4/backends/cpu/thread_pool.hpp>
#include <xmipp4/core/hardware/command.hpp>

#include <stdexcept>
#include <utility>

namespace xmipp4
{
namespace cpu
{

std::shared_ptr<command_queue> command_queue::m_instance;

command_queue::command_queue(std::shared_ptr<thread_pool> pool)
	: m_pool(
		pool ? std::move(pool) : thread_pool::get_default()
	)
{
}

void command_queue::submit(const command &cmd)
{
	const auto& prog = cmd.get_program();
	if (!prog)
	{
		throw std::invalid_argument(
			"command_queue::submit: Provided command does not have an "
			"associated program."
		);
	}

	// Synchronous, and it has to stay that way: the command holds spans the
	// caller owns, which stop being valid the moment this returns. Threading
	// a program does not change that, the pool being fork-join.
	const auto& cpu_prog = dynamic_cast<const program&>(*prog);
	cpu_prog.execute(
		cmd.get_outputs(),
		cmd.get_inputs(),
		cmd.get_scratch(),
		*m_pool
	);
}

thread_pool& command_queue::get_thread_pool() const noexcept
{
	return *m_pool;
}

void command_queue::signal(event &/*event*/)
{
	// No-op, synchronous execution.
}

void command_queue::wait(const event&)
{
	// No-op, synchronous execution.
}

std::shared_ptr<command_queue> command_queue::create()
{
	// The only state a command_queue holds is the pool it runs programs over,
	// and every queue of this backend shares the one the backend shares, so
	// there is still nothing to tell two of them apart.
	if (m_instance == nullptr)
	{
		m_instance = std::make_shared<command_queue>();
	}

	XMIPP4_ASSERT(m_instance);
	return m_instance;
}

command_queue*
command_queue::try_cast(xmipp4::command_queue &queue) noexcept
{
	if (&queue == m_instance.get())
	{
		return m_instance.get();
	}

	return dynamic_cast<command_queue*>(&queue);
}

const command_queue*
command_queue::try_cast(const xmipp4::command_queue &queue) noexcept
{
	if (&queue == m_instance.get())
	{
		return m_instance.get();
	}

	return dynamic_cast<const command_queue*>(&queue);
}

} // namespace cpu
} // namespace xmipp4
