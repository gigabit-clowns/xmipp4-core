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

} // namespace multidimensional
} // namespace xmipp4
