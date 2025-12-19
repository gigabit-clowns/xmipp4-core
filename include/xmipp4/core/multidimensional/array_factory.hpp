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
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array empty(
	array_descriptor descriptor, 
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
