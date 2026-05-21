// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/dynamic_shared_object.h>

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
	 * @brief Get the minimum scratch buffer size required to submit this
	 * command.
	 *
	 * Reports the amount of transient device-accessible memory, in bytes,
	 * that the command needs in order to execute. The caller must provide a
	 * scratch buffer of at least this size to
	 * @ref command_queue::submit; backends use it as workspace and may write
	 * to or read from it arbitrarily for the duration of the command. Its
	 * contents are undefined on entry and on completion, and the buffer may
	 * be reused across unrelated operations as long as it satisfies the size
	 * requirement.
	 *
	 * A return value of @c 0 indicates that the command needs no scratch
	 * memory; in that case the @c scratch argument of
	 * @ref command_queue::submit may be @c nullptr. The base
	 * implementation returns @c 0; concrete commands that require workspace
	 * must override this method.
	 *
	 * The value reported here is a property of the command instance and must
	 * not change over its lifetime, so callers can query it once and size
	 * their scratch buffers accordingly before submission.
	 *
	 * @return Minimum size, in bytes, of the scratch buffer that must be
	 * supplied at submission time, or @c 0 if none is required.
	 */
	virtual std::size_t get_required_scratch_size() const noexcept;
};

} // namespace hardware
} // namespace xmipp4
