// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/const_array_ref.hpp>

#include <cstddef>

namespace xmipp4
{

class execution_context;

/**
 * @brief Multiply two arrays as stacks of matrices.
 *
 * @param x The first array.
 * @param y The second array.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The array holding the product.
 *
 * @note The last two axes of each input are the matrix and the rest are a stack
 * of them, broadcast between the inputs. A one dimensional input is
 * promoted for the duration and the axis added is dropped again.
 */
XMIPP4_CORE_API
array matmul(
	const_array_ref x,
	const_array_ref y,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Contract two arrays along one axis, broadcasting the rest.
 *
 * @param x The first array.
 * @param y The second array.
 * @param axis The axis to contract along. Negative values refer to axes
 * from the end.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The array holding the contracted products.
 *
 * @note The inputs are broadcast together and the contracted axis is lost.
 * This differs from @ref matmul, which takes the last two axes as a
 * matrix.
 *
 * @see matmul
 */
XMIPP4_CORE_API
array vecdot(
	const_array_ref x,
	const_array_ref y,
	std::ptrdiff_t axis,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Contract two arrays along their last axis.
 *
 * @param x The first array.
 * @param y The second array.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The array holding the contracted products.
 */
XMIPP4_CORE_API
array vecdot(
	const_array_ref x,
	const_array_ref y,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Take the cross product of two arrays along one axis.
 *
 * @param x The first array.
 * @param y The second array.
 * @param axis The axis to take the product along. Negative values refer to
 * axes from the end. It must have three components.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The array holding the cross products.
 *
 * @note The inputs are broadcast together, so a stack of vectors crosses
 * with a single one without either being written out.
 */
XMIPP4_CORE_API
array cross(
	const_array_ref x,
	const_array_ref y,
	std::ptrdiff_t axis,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Take the cross product of two arrays along their last axis.
 *
 * @param x The first array.
 * @param y The second array.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The array holding the cross products.
 */
XMIPP4_CORE_API
array cross(
	const_array_ref x,
	const_array_ref y,
	const execution_context &context,
	array *out = nullptr
);

} // namespace xmipp4
