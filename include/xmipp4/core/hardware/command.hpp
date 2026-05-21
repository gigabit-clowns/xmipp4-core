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
	 * @brief Query the scratch buffer requirement for submitting this command.
	 *
	 * Reports the transient device-accessible workspace that the command
	 * needs in order to execute. The caller must provide a scratch buffer
	 * satisfying both the size and the alignment reported here when calling
	 * @ref command_queue::submit; backends use the buffer as workspace and
	 * may write to or read from it arbitrarily for the duration of the
	 * command. Its contents are undefined on entry and on completion, and
	 * the buffer may be reused across unrelated operations as long as it
	 * satisfies the size and alignment requirements.
	 *
	 * When this method returns @c false, the command requires no scratch
	 * memory: @p size and @p alignment are left untouched and the @c scratch
	 * argument of @ref command_queue::submit may be @c nullptr. When it
	 * returns @c true, @p size is set to the minimum required size in bytes
	 * (always non-zero) and @p alignment is set to the minimum required
	 * alignment in bytes (always a power of two, at least @c 1). The base
	 * implementation returns @c false; concrete commands that require
	 * workspace must override this method.
	 *
	 * The values reported here are a property of the command instance and
	 * must not change over its lifetime, so callers can query them once and
	 * size and align their scratch buffers accordingly before submission.
	 *
	 * @param[out] size Minimum scratch size in bytes. Written only when the 
	 * function returns @c true.
	 * @param[out] alignment Minimum scratch alignment in bytes, a power of
	 * two. Written only when the function returns @c true.
	 * @return @c true if the command requires a scratch buffer (and
	 * @p size / @p alignment have been written); @c false if no scratch is
	 * required.
	 */
	virtual 
	bool get_scratch_requirement(
		std::size_t &size, 
		std::size_t &alignment
	) const noexcept;
};

} // namespace hardware
} // namespace xmipp4
