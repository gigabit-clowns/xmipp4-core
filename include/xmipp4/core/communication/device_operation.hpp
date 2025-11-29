// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class device_queue;

} // namespace hardware

namespace communication
{

/**
 * @brief Abstract class to represent a device communication operation.
 * 
 * This serves as a base class for communication operations performed
 * with the device.
 * 
 */
class XMIPP4_CORE_API device_operation
	: public operation
{
public:
	device_operation() = default;
	device_operation(const device_operation &other) = delete;
	device_operation(device_operation &&other) = delete;
	~device_operation() override = default;

	device_operation& operator=(const device_operation &other) = delete;
	device_operation& operator=(device_operation &&other) = delete;

	/**
	 * @brief Execute the operation
	 * 
	 * @param queue The queue where this operation is executed. May be 
	 * null, in which case the operation is executed synchronously.
	 * 
	 * @note Calls to group's execute must be surrounded by 
	 * device_transaction::begin() and device_transaction::commit()
	 * 
	 */
	virtual void execute(hardware::device_queue *queue) = 0;
};

} // namespace communication
} // namespace xmipp4
