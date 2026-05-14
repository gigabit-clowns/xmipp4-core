// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device_queue.hpp>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief CPU implementation of @ref device_queue.
 *
 * This class is intended to be the entry point through which work is
 * dispatched onto a CPU thread pool. The pool is not wired in yet: the
 * queue currently has no in-flight work and therefore always reports
 * itself as idle. Once a thread pool exists, it will be plumbed through
 * this class and the synchronization methods below will track its work.
 */
class cpu_device_queue final
	: public device_queue
{
public:
	cpu_device_queue() = default;
	~cpu_device_queue() override = default;

	void wait_until_completed() const override;
	bool is_idle() const override;
};

} // namespace hardware
} // namespace xmipp4
