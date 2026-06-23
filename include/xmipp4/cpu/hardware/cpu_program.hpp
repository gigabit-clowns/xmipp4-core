// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/program.hpp>
#include <xmipp4/core/span.hpp>

namespace xmipp4
{
namespace hardware
{

class buffer;

/**
 * @brief CPU base class for executable programs.
 */
class XMIPP4_CORE_API cpu_program
	: public program
{
public:
	/**
	 * @brief Immediately execute the the program.
	 * 
	 * @param outputs Output buffers. Neither may be null.
	 * @param inputs Input buffers. Neither may be null.
	 * @param scratch Workspace buffers. Neither may be null.
	 */
	virtual void execute(
		span<const std::shared_ptr<buffer>> outputs,
		span<const std::shared_ptr<const buffer>> inputs,
		span<const std::shared_ptr<buffer>> scratch
	) const = 0;
};

} // namespace hardware
} // namespace xmipp4
