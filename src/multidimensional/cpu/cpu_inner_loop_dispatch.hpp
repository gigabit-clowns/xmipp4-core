// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array_access_layout.hpp>

#include <type_traits>
#include <cstddef>
#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

using contiguous_stride_tag = 
	std::integral_constant<std::ptrdiff_t, 1>;

using broadcasting_stride_tag = 
	std::integral_constant<std::ptrdiff_t, 0>;

/**
 * @brief Dispatch a function with potentially statically typed strides.
 * 
 * @tparam F Type of the functor to be called. Must accept a `std::size_t` and
 * a tuple of `N` integers or `std::integral_constant<std::ptrdiff_t, X>`.
 * @tparam N Statically defined number of operands in the array_access_layout.
 * @param callable The functor to be invoked. 
 * @param layout The layout to be dispatched.
 * @param n_operands Tag defining the number of operands.
 * @return auto The result of invoking the callable with the provided layout.
 */
template <typename F, std::size_t N>
auto dispatch_inner_loop(
	F &&callable, 
	const array_access_layout &layout, 
	std::integral_constant<std::size_t, N> n_operands
);

/**
 * @brief Dispatch a function with potentially statically typed strides.
 * 
 * @tparam F Type of the functor to be called. Must accept a `std::size_t` and
 * a tuple of `sizeof...(Is)` integers or 
 * `std::integral_constant<std::ptrdiff_t, X>`.
 * @tparam Is Indices of operands at which strides are extracted from the 
 * layout.
 * @param callable The functor to be invoked. 
 * @param layout The layout to be dispatched.
 * @param operand_indices Tag indicating operand indices from which strides
 * are extracted. 
 * @return auto The result of invoking the callable with the provided layout.
 */
template <typename F, std::size_t... Is>
auto dispatch_inner_loop(
	F &&callable, 
	const array_access_layout &layout, 
	std::index_sequence<Is...> operand_indices
);

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_inner_loop_dispatch.inl"
