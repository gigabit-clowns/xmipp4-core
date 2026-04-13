// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"

namespace xmipp4 
{

class execution_context;

namespace hardware
{

class memory_resource;

} // namespace hardware

namespace multidimensional
{

class array_view;

/**
 * @brief Transfer an array from host to device memory.
 * 
 * If the array is already device-accessible, this operation returns an
 * alias of the input array. Otherwise it behaves like `host_to_device_copy`
 * 
 * @param input Array to be transferred.
 * @param context The device context to handle the allocation.
 * @return array The transferred array.
 * 
 * @warning The returned array may alias the input array. Thus modifications
 * to it may have side-effects on the input array and vice-versa. 
 */
XMIPP4_CORE_API
array to_device(array &input, const execution_context &context);

/**
 * @brief Transfer an array to device memory enforcing a copy.
 * 
 * @param input Array to be transferred.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The transferred array.
 */
XMIPP4_CORE_API
array to_device_copy(
	array_view input, 
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Transfer an array to host accessible memory.
 * 
 * If the array is already gost-accessible, this operation returns an
 * alias of the input array. Otherwise it behaves like `device_to_host_copy`
 * 
 * @param input Array to be transferred.
 * @param context The device context to handle the allocation.
 * @return array The transferred array.
 * 
 * @warning The returned array may alias the input array. Thus modifications
 * to it may have side-effects on the input array and vice-versa. 
 */
XMIPP4_CORE_API
array to_host(array &input, const execution_context &context);

/**
 * @brief Transfer an array to host accessible memory enforcing a copy.
 * 
 * @param input Array to be transferred.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The transferred array.
 */
XMIPP4_CORE_API
array to_host_copy(
	array_view input, 
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
