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
 * @brief Evaluate `x ^ y` for each element in the arrays.
 *
 * @param base The base of the power operation. 
 * @param exponent The exponent of the power operation. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array pow(
	const array_view &base,
	const array_view &exponent,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `exp(x)` for each element in the arrays.
 *
 * @param x The exponent. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array exp(
	const array_view &x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `log(x)` for each element in the arrays.
 *
 * @param x The input array. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array exp(
	const array_view &x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x^2` for each element in the arrays.
 *
 * @param x The input array. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array square(
	const array_view &x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `sqrt(x)` for each element in the arrays.
 *
 * @param x The input array. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array sqrt(
	const array_view &x,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
