// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/command_queue.hpp>

namespace xmipp4
{
namespace cpu
{

/**
 * @brief CPU implementation of @ref command_queue.
 */
class command_queue final
	: public xmipp4::command_queue
{
public:
	command_queue() = default;
	~command_queue() override = default;

	void submit(const command &cmd) override;
	void signal(event &event) override;
	void wait(const event &event) override;

	static std::shared_ptr<command_queue> create();
	static 
	command_queue* try_cast(xmipp4::command_queue &queue) noexcept;
	static 
	const command_queue* try_cast(const xmipp4::command_queue &queue) noexcept;

private:
	static std::shared_ptr<command_queue> m_instance;
};

} // namespace cpu
} // namespace xmipp4
