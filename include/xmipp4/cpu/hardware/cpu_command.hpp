// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/command.hpp>
#include <xmipp4/core/span.hpp>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief CPU base class for executable commands.
 *
 * Concrete CPU commands derive from this class and implement @ref execute,
 * which receives raw host pointers to the operand buffers as supplied by
 * @ref cpu_command_queue::submit. The queue extracts these pointers from
 * the @ref buffer operands via @ref buffer::get_host_ptr before invoking
 * @ref execute, so command implementations work directly on host memory
 * and need not be aware of the @ref buffer abstraction.
 *
 * Operand ordering matches the read-write / read-only descriptors fixed at
 * command construction, as documented on @ref command_queue::submit.
 */
class cpu_command
	: public command
{
public:
	/**
	 * @brief Run the command on the given host operands.
	 *
	 * @param outputs Host pointers to the read-write operand buffers, in the
	 * order fixed at command construction. None of the pointers may be null.
	 * @param inputs  Host pointers to the read-only operand buffers, in the
	 * order fixed at command construction. None of the pointers may be null.
	 */
	virtual void execute(
		span<void* const> outputs,
		span<const void* const> inputs
	) const = 0;
};

} // namespace hardware
} // namespace xmipp4
