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
class operation_command_manager;
class operation_command_cache;

XMIPP4_CORE_API
void execute(
	const operation &operation,
	span<array> output_operands,
	span<const array_view> input_operands,
	const execution_context &context
);

} // namespace multidimensional
} // namespace xmipp4
