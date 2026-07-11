// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/command_queue.hpp>

namespace xmipp4
{

/**
 * @brief CPU implementation of @ref command_queue.
 */
class cpu_command_queue final
	: public command_queue
{
public:
	cpu_command_queue() = default;
	~cpu_command_queue() override = default;

	void submit(const command &cmd) override;
	void signal(event &event) override;
	void wait(const event &event) override;

	static std::shared_ptr<cpu_command_queue> create();
	static 
	cpu_command_queue* try_cast(command_queue &queue) noexcept;
	static 
	const cpu_command_queue* try_cast(const command_queue &queue) noexcept;

private:
	static std::shared_ptr<cpu_command_queue> m_instance;
};

} // namespace xmipp4
