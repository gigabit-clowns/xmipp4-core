// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "command_scratch_requirement.hpp"

#include <xmipp4/core/span.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <vector>
#include <cstddef>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief Abstract representation of an executable command.
 *
 * A @c command is a passive and backend-specific descriptor of work to 
 * be executed on a device. Execution is driven by @ref command_queue::submit,
 * which records the command on a queue together with its operands. 
 *
 * Each concrete command instance is bound to the device it was created for
 * and may only be submitted to queues belonging to that device.
 */
class command
{
public:
	XMIPP4_CORE_API
	command() noexcept;
	command(const command &other) = delete;
	command(command &&other) = delete;
	XMIPP4_CORE_API
	virtual ~command();
	
	command& operator=(const command &other) = delete;
	command& operator=(command &&other) = delete;

	/**
	 * @brief Queries the scratch buffer requirements for this command.
	 *
	 * Commands may require temporary (scratch) memory buffers during
	 * execution. This method populates the provided vector with all scratch
	 * buffer requirements for this command, allowing the caller to allocate
	 * the necessary buffers before command submission.
	 *
	 * If the command does not require any scratch buffers, the vector will
	 * be empty upon return.
	 *
	 * @return A vectospan with the scratch requirements for this command.
	 *
	 * @see command_scratch_requirement for details on individual requirement 
	 * specifications.
	 */
	virtual
	span<const command_scratch_requirement> 
	get_scratch_requirements() const noexcept;
};

} // namespace hardware
} // namespace xmipp4
