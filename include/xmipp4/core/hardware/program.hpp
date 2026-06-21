// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "program_scratch_requirement.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <vector>
#include <cstddef>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief Abstract representation of an executable program.
 *
 * A @c program is a passive and backend-specific descriptor of work to
 * be executed on a device. Execution is driven by @ref command_queue::submit,
 * which records the program on a queue together with its operands.
 *
 * Each concrete program instance is bound to the device it was created for
 * and may only be submitted to queues belonging to that device.
 */
class program
{
public:
	XMIPP4_CORE_API
	program() noexcept;
	program(const program &other) = delete;
	program(program &&other) = delete;
	XMIPP4_CORE_API
	virtual ~program();

	program& operator=(const program &other) = delete;
	program& operator=(program &&other) = delete;

	/**
	 * @brief Queries the scratch buffer requirements for this program.
	 *
	 * Programs may require temporary (scratch) memory buffers during
	 * execution. This method populates the provided vector with all scratch
	 * buffer requirements for this program, allowing the caller to allocate
	 * the necessary buffers before program submission.
	 *
	 * If the program does not require any scratch buffers, the vector will
	 * be empty upon return.
	 *
	 * @param[out] requirements A vector to be populated with the scratch
	 * buffer requirements for this program. The vector is cleared before being
	 * populated with the program's requirements.
	 *
	 * @see program_scratch_requirement for details on individual requirement
	 * specifications.
	 */
	virtual
	void get_scratch_requirements(
		std::vector<program_scratch_requirement> &requirements
	) const;
};

} // namespace hardware
} // namespace xmipp4
