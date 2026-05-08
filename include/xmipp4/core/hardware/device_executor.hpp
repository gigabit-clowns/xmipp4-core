// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

/**
 * @brief Abstract class describing a command queue or stream.
 * 
 */
class XMIPP4_CORE_API device_executor
{
public:
	device_executor() noexcept;
	device_executor(const device_executor &other) = delete;
	device_executor(device_executor &&other) = delete;
	virtual ~device_executor();

	device_executor& operator=(const device_executor &other) = delete;
	device_executor& operator=(device_executor &&other) = delete;

	/**
	 * @brief Wait until the device_executor is flushed.
	 *
	 */
	virtual void wait_until_completed() const = 0;

	/**
	 * @brief Check if the executor has completed processing.
	 *
	 * @return true if executor has finished processing.
	 * @return false if executor is busy processing.
	 */
	virtual bool is_idle() const noexcept = 0;
}; 

} // namespace hardware
} // namespace xmipp4
