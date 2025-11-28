// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace communication
{

/**
 * @brief Abstract class to represent a communication operation.
 * 
 * This class provides the common interface shared by host_operation
 * and device_operation. 
 * 
 */
class XMIPP4_CORE_API operation
{
public:
	operation() = default;
	operation(const operation &other) = delete;
	operation(operation &&other) = delete;
	virtual ~operation() = default;

	operation& operator=(const operation &other) = delete;
	operation& operator=(operation &&other) = delete;

};

} // namespace communication
} // namespace xmipp4

