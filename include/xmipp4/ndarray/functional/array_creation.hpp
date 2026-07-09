// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/array_view.hpp>

#include <xmipp4/core/scalar_value.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>

namespace xmipp4
{
namespace dispatch
{

class execution_context;

} // namespace dispatch

namespace ndarray
{

/**
 * @brief Create an array without initializing its elements.
 *
 * @param descriptor Describes the shape and data type of the array.
 * @param affinity Affinity used to select the memory resource where the
 * array is allocated.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The newly created array.
 */
XMIPP4_CORE_API
ndarray::array empty(
	ndarray::array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const dispatch::execution_context &context,
	ndarray::array *out = nullptr
);

/**
 * @brief Create an array with all its elements set to zero.
 *
 * @param descriptor Describes the shape and data type of the array.
 * @param affinity Affinity used to select the memory resource where the
 * array is allocated.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The newly created array.
 */
XMIPP4_CORE_API
ndarray::array zeros(
	ndarray::array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const dispatch::execution_context &context,
	ndarray::array *out = nullptr
);

/**
 * @brief Create an array with all its elements set to one.
 *
 * @param descriptor Describes the shape and data type of the array.
 * @param affinity Affinity used to select the memory resource where the
 * array is allocated.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The newly created array.
 */
XMIPP4_CORE_API
ndarray::array ones(
	ndarray::array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const dispatch::execution_context &context,
	ndarray::array *out = nullptr
);

/**
 * @brief Create an array with all its elements set to a given value.
 *
 * @param descriptor Describes the shape and data type of the array.
 * @param affinity Affinity used to select the memory resource where the
 * array is allocated.
 * @param fill_value Value to which all the elements are set.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The newly created array.
 */
XMIPP4_CORE_API
ndarray::array full(
	ndarray::array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const scalar_value &fill_value,
	const dispatch::execution_context &context,
	ndarray::array *out = nullptr
);

/**
 * @brief Create an array as a copy of an existing one.
 *
 * @param source The array to be copied.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The newly created array.
 */
XMIPP4_CORE_API
ndarray::array copy(
	ndarray::array_view source,
	const dispatch::execution_context &context,
	ndarray::array *out = nullptr
);

/**
 * @brief Set all the elements of an array to a given value.
 *
 * @param out The array whose elements are set.
 * @param fill_value Value to which all the elements are set.
 * @param context The execution context used for dispatching.
 * @return array The filled array.
 */
XMIPP4_CORE_API
void fill(
	ndarray::array &out,
	const scalar_value &fill_value,
	const dispatch::execution_context &context
);

} // namespace ndarray
} // namespace xmipp4
