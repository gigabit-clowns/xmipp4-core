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
array transfer(
	array &source, 
	hardware::memory_resource_affinity target_affinity,
	const execution_context &context
);

XMIPP4_CORE_API
array transfer_copy(
	array_view source,
	hardware::memory_resource_affinity target_affinity,
	const execution_context &context,
	array *out = nullptr
);
} // namespace multidimensional
} // namespace xmipp4
