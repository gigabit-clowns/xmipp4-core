// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"

namespace xmipp4 
{

class execution_context;

namespace multidimensional
{

class array_view;

/**
 * @brief Evaluate `-x` for each element in the arrays.
 *
 * @param x Array to be negated. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array negate(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `abs(x)` for each element in the arrays.
 *
 * @param x Input array. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The absolute value of the input elements. For complex types
 * this corresponds to the magnitude.
 */
XMIPP4_CORE_API
array abs(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `conj(x)` for each element in the arrays.
 *
 * @param x Input array. Must have complex data type.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The conjugate values of the input array.
 */
XMIPP4_CORE_API
array conj(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x + y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting addition.
 */
XMIPP4_CORE_API
array add(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x - y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting subtraction.
 */
XMIPP4_CORE_API
array subtract(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x * y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result of the multiplication.
 */
XMIPP4_CORE_API
array multiply(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x / y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result of the division.
 */
XMIPP4_CORE_API
array divide(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
