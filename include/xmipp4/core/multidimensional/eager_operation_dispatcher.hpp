// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class device_context;

} // namespace hardware

namespace multidimensional
{

class operation;
class array;
class kernel_manager;

class eager_operation_dispatcher
{
public:
	XMIPP4_CORE_API
	eager_operation_dispatcher(kernel_manager &manager) noexcept;
	eager_operation_dispatcher(const eager_operation_dispatcher &other) = delete;
	eager_operation_dispatcher(eager_operation_dispatcher &&other) = delete;
	XMIPP4_CORE_API
	~eager_operation_dispatcher();

	eager_operation_dispatcher& 
	operator=(const eager_operation_dispatcher &other) = delete;
	eager_operation_dispatcher& 
	operator=(eager_operation_dispatcher &&other) = delete;

	XMIPP4_CORE_API
	void dispatch(
		const operation &operation,
		span<array> output_operands,
		span<const array> input_operands,
		const hardware::device_context &device_context
	);

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

};

} // namespace multidimensional
} // namespace xmipp4
