// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"

namespace xmipp4
{

class array;
class array_view;


class operation;
class execution_context;

/**
 * @brief Execute a operation.
 *
 * @param operation The operation to be executed.
 * @param output_operands The output operands.
 * @param input_operands The input operands.
 * @param context The execution context used for dispatching.
 */
XMIPP4_CORE_API
void execute(
	const operation &operation,
	span<array> output_operands,
	span<const array_view> input_operands,
	const execution_context &context
);

/**
 * @brief Execute a operation.
 *
 * @param operation The operation to be executed.
 * @param input_operands The input operands.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 */
XMIPP4_CORE_API
array execute(
	const operation &operation,
	span<const array_view> input_operands,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Execute an unary operation.
 *
 * @param operation The operation to be executed.
 * @param input The input operand.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 */
XMIPP4_CORE_API
array execute_unary(
	const operation &operation,
	const array_view &input,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Execute an unary operation.
 *
 * @param operation The operation to be executed.
 * @param first_input The first input operand.
 * @param second_input The second input operand.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 */
XMIPP4_CORE_API
array execute_binary(
	const operation &operation,
	array_view first_input,
	array_view second_input,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Execute an unary operation.
 *
 * @param operation The operation to be executed.
 * @param first_input The first input operand.
 * @param second_input The second input operand.
 * @param third_input The third input operand.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 */
XMIPP4_CORE_API
array execute_ternary(
	const operation &operation,
	array_view first_input,
	array_view second_input,
	array_view third_input,
	const execution_context &context,
	array *out = nullptr
);

} // namespace xmipp4
