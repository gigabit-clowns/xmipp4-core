// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../ndarray/array.hpp"
#include "../ndarray/array_view.hpp"

#include <xmipp4/core/hardware/memory_resource_affinity.hpp>

namespace xmipp4
{
namespace execution
{

class context;

} // namespace execution

namespace ndarray
{

/**
 * @brief Transfer an array to memory with the requested affinity.
 *
 * If the array is already accessible from a memory resource with the
 * requested affinity, this operation returns an alias of the input array.
 * Otherwise it behaves like @ref transfer_copy.
 *
 * @param input Array to be transferred.
 * @param affinity Affinity of the memory resource to transfer the array to.
 * @param context The execution environment.
 * @return array The transferred array.
 *
 * @warning The returned array may alias the input array. Thus modifications
 * to it may have side-effects on the input array and vice-versa. Prefer
 * @ref transfer_copy if this is a concern.
 */
XMIPP4_CORE_API
ndarray::array transfer(
	ndarray::array &input,
	hardware::memory_resource_affinity affinity,
	const execution::context &context
);

/**
 * @brief Transfer an array to memory with the requested affinity enforcing a
 * copy.
 *
 * Unlike @ref transfer, this operation always allocates fresh storage and
 * copies the input into it, so the result never aliases the input.
 *
 * @param input Array to be transferred.
 * @param affinity Affinity of the memory resource to transfer the array to.
 * @param context The execution environment.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The transferred array.
 */
XMIPP4_CORE_API
ndarray::array transfer_copy(
	ndarray::array_view input,
	hardware::memory_resource_affinity affinity,
	const execution::context &context,
	ndarray::array *out = nullptr
);

/**
 * @brief Transfer an array from host to device memory.
 * 
 * If the array is already device-accessible, this operation returns an
 * alias of the input array. Otherwise it behaves like `to_device_copy`
 * 
 * @param input Array to be transferred.
 * @param context The execution environment.
 * @return array The transferred array.
 * 
 * @warning The returned array may alias the input array. Thus modifications
 * to it may have side-effects on the input array and vice-versa. Prefer 
 * @ref to_device_copy if this is a concern.
 */
XMIPP4_CORE_API
ndarray::array to_device(
	ndarray::array &input,
	const execution::context &context
);

/**
 * @brief Transfer an array to device memory enforcing a copy.
 * 
 * @param input Array to be transferred.
 * @param context The execution environment.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The transferred array.
 */
XMIPP4_CORE_API
ndarray::array to_device_copy(
	ndarray::array_view input,
	const execution::context &context,
	ndarray::array *out = nullptr
);

/**
 * @brief Transfer an array to host accessible memory.
 * 
 * If the array is already host-accessible, this operation returns an
 * alias of the input array. Otherwise it behaves like `to_host_copy`
 * 
 * @param input Array to be transferred.
 * @param context The execution environment.
 * @return array The transferred array.
 * 
 * @warning The returned array may alias the input array. Thus modifications
 * to it may have side-effects on the input array and vice-versa. Prefer 
 * @ref to_host_copy if this is a concern. 
 */
XMIPP4_CORE_API
ndarray::array to_host(
	ndarray::array &input,
	const execution::context &context
);

/**
 * @brief Transfer an array to host accessible memory enforcing a copy.
 * 
 * @param input Array to be transferred.
 * @param context The execution environment.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The transferred array.
 */
XMIPP4_CORE_API
ndarray::array to_host_copy(
	ndarray::array_view input,
	const execution::context &context,
	ndarray::array *out = nullptr
);

} // namespace ndarray
} // namespace xmipp4
