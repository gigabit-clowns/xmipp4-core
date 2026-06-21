// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/program.hpp>
#include <xmipp4/core/span.hpp>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief CPU base class for executable programs.
 */
class XMIPP4_CORE_API cpu_program
	: public program
{
public:
	/**
	 * @brief Run the program on the given host operands.
	 *
	 * @param outputs Host pointers to the read-write operands. Must not be null.
	 * @param inputs  Host pointers to the read-only operands. Must not be null.
	 * @param scratch Host pointers to the read-write workspaces. Must not be
	 * null.
	 */
	virtual void execute(
		span<void* const> outputs,
		span<const void* const> inputs,
		span<void* const> scratch
	) const = 0;
};

} // namespace hardware
} // namespace xmipp4
