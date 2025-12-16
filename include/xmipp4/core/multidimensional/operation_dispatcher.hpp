// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"

namespace xmipp4 
{
namespace hardware
{

class device_context;
	
} // namespace hardware


namespace multidimensional
{

class array;
class operation;

class operation_dispatcher
{
public:
	XMIPP4_CORE_API
	operation_dispatcher() noexcept;
	operation_dispatcher(const operation_dispatcher &other) = delete;
	operation_dispatcher(operation_dispatcher &&other) = delete;
	XMIPP4_CORE_API
	virtual ~operation_dispatcher();

	operation_dispatcher& operator=(const operation_dispatcher &other) = delete;
	operation_dispatcher& operator=(operation_dispatcher &&other) = delete;

	virtual void dispatch(
		const operation &operation,
		span<array> output_operands,
		span<const array> input_operands,
		const hardware::device_context &device_context
	) = 0;
};

} // namespace multidimensional
} // namespace xmipp4

