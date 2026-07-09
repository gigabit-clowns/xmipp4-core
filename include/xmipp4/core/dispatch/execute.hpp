// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"

namespace xmipp4
{
namespace ndarray
{

class array;
class array_view;

} // namespace ndarray

namespace dispatch
{

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
	span<ndarray::array> output_operands,
	span<const ndarray::array_view> input_operands,
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
ndarray::array execute(
	const operation &operation,
	span<const ndarray::array_view> input_operands,
	const execution_context &context,
	ndarray::array *out = nullptr
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
ndarray::array execute_unary(
	const operation &operation,
	const ndarray::array_view &input,
	const execution_context &context,
	ndarray::array *out = nullptr
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
ndarray::array execute_binary(
	const operation &operation,
	ndarray::array_view first_input,
	ndarray::array_view second_input,
	const execution_context &context,
	ndarray::array *out = nullptr
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
ndarray::array execute_ternary(
	const operation &operation,
	ndarray::array_view first_input,
	ndarray::array_view second_input,
	ndarray::array_view third_input,
	const execution_context &context,
	ndarray::array *out = nullptr
);

} // namespace dispatch
} // namespace xmipp4
