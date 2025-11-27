// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace communication
{

/**
 * @brief Abstract class to represent a host communication operation.
 * 
 * This serves as a base class for communication operations performed
 * with the host.
 * 
 */
class XMIPP4_CORE_API host_operation
	: public operation
{
public:
	host_operation() = default;
	host_operation(const host_operation &other) = delete;
	host_operation(host_operation &&other) = delete;
	~host_operation() override = default;

	host_operation& operator=(const host_operation &other) = delete;
	host_operation& operator=(host_operation &&other) = delete;

	/**
	 * @brief Execute the operation.
	 * 
	 */
	virtual void execute() = 0;

};

} // namespace communication
} // namespace xmipp4
