// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"

namespace xmipp4 
{

class execution_context;

namespace multidimensional
{

class array;
class array_view;
class operation;

/**
 * @brief Abstract class responsible of dispatching operations.
 * 
 */
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

	/**
	 * @brief Dispatch the provided operation.
	 * 
	 * @param operation The operation to be dispatched.
	 * @param output_operands The output operands of the operation. If provided
	 * in a default constructed or invalid, their descriptors are inferred and t
	 * heir storage is allocated. 
	 * @param input_operands The input operands of the operation. Their 
	 * descriptors must be consistent with the invariance defined by the 
	 * operation and their storages must be accessible by the device used to 
	 * execute the operation.
	 * @param context The execution context containing all the ancillary 
	 * objects required for dispatch.
	 */
	virtual void dispatch(
		const operation &operation,
		span<array> output_operands,
		span<const array_view> input_operands,
		const execution_context &context
	) = 0;
};

} // namespace multidimensional
} // namespace xmipp4

