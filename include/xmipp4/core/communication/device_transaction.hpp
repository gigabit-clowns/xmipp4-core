// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace communication
{

/**
 * @brief Abstract class to represent a transaction simultaneously happening 
 * to multiple device_communicator-s
 * 
 */
class XMIPP4_CORE_API device_transaction
{
public:
	device_transaction() = default;
	device_transaction(const device_transaction &other) = delete;
	device_transaction(device_transaction &&other) = delete;
	virtual ~device_transaction() = default;

	device_transaction& operator=(const device_transaction &other) = delete;
	device_transaction& operator=(device_transaction &&other) = delete;

	/**
	 * @brief Begin the transaction. 
	 * 
	 * Must be called before a series of device_operation::execute()
	 * 
	 */
	virtual void begin() = 0;

	/**
	 * @brief Begin the transaction. 
	 * 
	 * Must be called at the end of a series of device_operation::execute()
	 * 
	 */
	virtual void commit() = 0;
};

} // namespace communication
} // namespace xmipp4

