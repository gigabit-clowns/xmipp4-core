// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "../hardware/memory_resource_affinity.hpp"

namespace xmipp4 
{

class execution_context;

namespace multidimensional
{

XMIPP4_CORE_API
array cast(
	array &source, 
	numerical_type target_type,
	const execution_context &context
);

XMIPP4_CORE_API
array cast_copy(
	array_view source,
	numerical_type target_type,
	const execution_context &context,
	array *out = nullptr
);
} // namespace multidimensional
} // namespace xmipp4
