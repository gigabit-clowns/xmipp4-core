// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief Abstract representation of an executable command.
 *
 * A @c command is a passive, inmutable, backend-specific descriptor of work to 
 * be executed on a device. Execution is driven by @ref command_queue::submit,
 * which records the command on a queue together with its operands. The
 * read-write and read-only operand descriptors fixed at construction
 * determine the layout that @ref command_queue::submit expects.
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
};

} // namespace hardware
} // namespace xmipp4
