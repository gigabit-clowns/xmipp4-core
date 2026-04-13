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
 * @brief Add together the values in the provided axes.
 *
 * @param x Array to be added.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keepdim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting summation.
 */
XMIPP4_CORE_API
array sum(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

/**
 * @brief Get the maximum element over a set of axes.
 *
 * @param x Array where the maximum elements are searched.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keepdim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result.
 */
XMIPP4_CORE_API
array max(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

/**
 * @brief Get the minimum element over a set of axes.
 *
 * @param x Array where the minimum elements are searched.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keep_dim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result.
 */
XMIPP4_CORE_API
array min(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

/**
 * @brief Get the mean over a set of axes.
 *
 * The energy is defined as `sum(x, axes, where) / N`. Where `N` is the number 
 * of elements.
 * 
 * @param x Array where the mean is computed.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keep_dim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result.
 */
XMIPP4_CORE_API
array mean(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

/**
 * @brief Get the energy over a set of axes.
 *
 * The energy is defined as `sum(square(x), axes, where)`.
 * 
 * @param x Array where the energy is computed.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keep_dim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result.
 */
XMIPP4_CORE_API
array energy(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

/**
 * @brief Get the power over a set of axes.
 *
 * The power is defined as `sum(square(x), axes, where) / N` where `N` is the 
 * number of elements.
 * 
 * @param x Array where the power is computed.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keep_dim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result.
 */
XMIPP4_CORE_API
array power(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

/**
 * @brief Get the power over a set of axes.
 *
 * The rms value is defined as `sqrt(sum(square(x), axes, where) / N)` where 
 * `N` is the number of elements.
 * 
 * @param x Array where the power is computed.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keep_dim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result.
 */
XMIPP4_CORE_API
array rms(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

/**
 * @brief Get the power over a set of axes.
 *
 * The norm is defined as `sqrt(sum(square(x), axes, where))`.
 * 
 * @param x Array where the power is computed.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keep_dim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result.
 */
XMIPP4_CORE_API
array norm(
	const array_view &x,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

/**
 * @brief Get the sum of the products of two arrays.
 *
 * This is similar to the dot product, except that it can be computed for
 * multiple axes. The sumproduct is defined as `sum(x*y, axes, where)`.
 * 
 * @param x First array.
 * @param y Second array.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keep_dim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The dot product.
 */
XMIPP4_CORE_API
array sumproduct(
	const array_view &x,
	const array_view &y,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

/**
 * @brief Get the euclidean distance between to arrays.
 *
 * The squared euclidean distance is defined as 
 * `sqrt(sum(square(x - y), axes, where))`.
 * 
 * @param x First array.
 * @param y Second array.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keep_dim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The euclidean distance computation.
 */
XMIPP4_CORE_API
array euclidean_distance(
	const array_view &x,
	const array_view &y,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

/**
 * @brief Get the squared euclidean distance between to arrays.
 *
 * The squared euclidean distance is defined as 
 * `sum(square(x - y), axes, where)`.
 * 
 * @param x First array.
 * @param y Second array.
 * @param axes The axes on which elements are added.
 * @param context The device context to handle the allocation.
 * @param keep_dim When false, the reduced axes are removed from the output. If 
 * true reduced axes are replaced by axes with extent 1.
 * @param where Optional masking array. If provided, the operation is performed
 * on elements where this array evaluates to true. Must be broadcastble with
 * the input array.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The squared euclidean distance computation.
 */
XMIPP4_CORE_API
array euclidean_distance2(
	const array_view &x,
	const array_view &y,
	span<const std::size_t> axes,
	const execution_context &context,
	bool keep_dim = false,
	const array_view *where = nullptr,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
