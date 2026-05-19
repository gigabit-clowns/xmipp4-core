// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/command_queue.hpp>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief CPU implementation of @ref command_queue.
 *
 * This class is intended to be the entry point through which work is
 * dispatched onto a CPU thread pool. The pool is not wired in yet: the
 * queue currently has no in-flight work and therefore always reports
 * itself as idle. Once a thread pool exists, it will be plumbed through
 * this class and the synchronization methods below will track its work.
 */
class cpu_command_queue final
	: public command_queue
{
public:
	cpu_command_queue() = default;
	~cpu_command_queue() override = default;

	void wait_until_completed() const override;
	bool is_idle() const override;

	void signal(event &event) override;
	void wait(const event &event) override;

	void submit(
		const command &command,
		span<const std::shared_ptr<buffer>> output_operands,
		span<const std::shared_ptr<const buffer>> input_operands
	) override;
};

} // namespace hardware
} // namespace xmipp4
