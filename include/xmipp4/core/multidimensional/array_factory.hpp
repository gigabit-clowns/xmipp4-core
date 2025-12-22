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
 * @brief Create an array with uninitialized contents.
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

/**
 * @brief Create an array full of zeros.
 * 
 * @param descriptor The descriptor for the new array.
 * @param affinity The affinity for the array's data.
 * @param context The device context to handle the allocation and dispatch.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting array filled with zeros.
 */
XMIPP4_CORE_API
array zeros(
	array_descriptor descriptor, 
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Create an array full of ones.
 * 
 * @param descriptor The descriptor for the new array.
 * @param affinity The affinity for the array's data.
 * @param context The device context to handle the allocation and dispatch.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting array filled with ones.
 */
XMIPP4_CORE_API
array ones(
	array_descriptor descriptor, 
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Create an array full of an arbitrary value.
 * 
 * @param descriptor The descriptor for the new array.
 * @param affinity The affinity for the array's data.
 * @param fill_value The value to fill the array with.
 * @param context The device context to handle the allocation and dispatch.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting array filled with the specified value.
 */
XMIPP4_CORE_API
array full(
	array_descriptor descriptor, 
	hardware::memory_resource_affinity affinity,
	/*TODO: fill value*/
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Create an array by copying the contents of another array.
 * 
 * @param source The array whose contents are copied.
 * @param context The device context to handle the allocation and dispatch.
 * @param out Optional array to which contents are copied. If provided, the
 * input is broadcasted to match the shape of this array. Therefore, input
 * must be broadcastable to out's shape.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array copy(
	array_view source,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
