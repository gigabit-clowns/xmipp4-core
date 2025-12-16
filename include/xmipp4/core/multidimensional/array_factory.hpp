// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "../hardware/device.hpp"

namespace xmipp4 
{

class execution_context;

namespace multidimensional
{


/**
 * @brief Create an empty array for a given descriptor.
 * 
 * @param descriptor The descriptor for the new array.
 * @param affinity The affinity for the array's data.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its storage may be aliased
 * in the new array, avoiding memory allocations.
 * @return array The resulting empty array.
 */
XMIPP4_NODISCARD XMIPP4_CORE_API
array empty(
	array_descriptor descriptor, 
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
