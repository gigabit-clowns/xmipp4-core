// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/command_queue.hpp>

#include <memory>

namespace xmipp4
{
namespace cpu
{

class thread_pool;

/**
 * @brief CPU implementation of @ref command_queue.
 */
class command_queue final
	: public xmipp4::command_queue
{
public:
	/**
	 * @brief Construct a queue over a set of threads.
	 *
	 * @param pool The threads the programs submitted here may spread
	 * themselves over. Defaults to the pool the backend shares, and falls
	 * back to it when null. Naming one is what lets a test drive a whole
	 * program through a known number of threads.
	 */
	explicit command_queue(
		std::shared_ptr<thread_pool> pool = nullptr
	);
	~command_queue() override = default;

	void submit(const command &cmd) override;
	void signal(event &event) override;
	void wait(const event &event) override;

	/**
	 * @brief Get the threads this queue runs its programs over.
	 *
	 * @return thread_pool& The pool.
	 */
	thread_pool& get_thread_pool() const noexcept;

	static std::shared_ptr<command_queue> create();
	static
	command_queue* try_cast(xmipp4::command_queue &queue) noexcept;
	static
	const command_queue* try_cast(const xmipp4::command_queue &queue) noexcept;

private:
	std::shared_ptr<thread_pool> m_pool;

	static std::shared_ptr<command_queue> m_instance;
};

} // namespace cpu
} // namespace xmipp4
