// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "../hardware/memory_resource_affinity.hpp"

namespace xmipp4 
{

class execution_context;

namespace multidimensional
{

/**
 * @brief Transfer an array to a different memory resource.
 * 
 * When the source array is already located in the requested memory resource,
 * an alias to the original array is returned. Otherwise, a copy of the array
 * is performed to a new array located in the requested memory resource. Note
 * that in unified memory architectures, this function also retuns an alias.
 * 
 * @param source The array to be transferred.
 * @param target_affinity Target placement for the transferred array.
 * @param context The execution context where the transfer will be performed.
 * @return array The transferred array.
 */
XMIPP4_CORE_API
array transfer(
	array &source, 
	hardware::memory_resource_affinity target_affinity,
	const execution_context &context
);

/**
 * @brief Transfer an array to a different memory resource by copying its 
 * contents.
 * 
 * Unlike `transfer`, this function always performs a copy of the source array,
 * regardless of its current placement.
 * 
 * @param source The array to be transferred.
 * @param target_affinity Target placement for the transferred array.
 * @param context The execution context where the transfer will be performed.
 * @param out Optional array to reuse for the result.
 * @return array The transferred array.
 */
XMIPP4_CORE_API
array transfer_copy(
	array_view source,
	hardware::memory_resource_affinity target_affinity,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
