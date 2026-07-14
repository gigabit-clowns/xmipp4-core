// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/layout/joint_layout.hpp>

#include <cstddef>

namespace xmipp4
{
namespace cpu
{

/**
 * @brief Run an elementwise loop over a layout, statically dispatching each
 * operand's inner-loop stride.
 *
 * Composes @ref dispatch_inner_loop_strides and @ref run_elementwise_outer_loop:
 * each operand's inner stride is resolved to a stride tag once, then the outer
 * dimensions of @p layout are iterated, invoking @p kernel once per contiguous
 * 1D vector. This is the flat building block shared by CPU elementwise
 * operation program builders, replacing the nested stride-dispatch and
 * outer-loop lambdas with a single kernel.
 *
 * The kernel is invoked as:
 * @code
 * kernel(pointers, strides, count)
 * @endcode
 * where `pointers` is a `std::tuple` of the operand pointers advanced to the
 * start of the current 1D vector, `strides` is a `std::tuple` of the
 * statically resolved inner strides (one per operand, in the same operand
 * order as `pointers`; each element is a `contiguous_stride_tag`, a
 * `broadcasting_stride_tag` or a runtime `std::ptrdiff_t`) and `count` is the
 * number of elements in the vector.
 *
 * @tparam Kernel Functor invoked per 1D vector. Must accept
 * `(std::tuple<Pointers...>, std::tuple<Strides...>, std::size_t)`.
 * @tparam Pointers CV-qualified operand pointers, one per operand.
 * @param kernel The functor to be invoked for each 1D vector.
 * @param layout Access layout used both for stride dispatch and for iterating
 * over the operands.
 * @param pointers The base pointer of each operand.
 *
 * @warning Inherits the stride specialization cost of
 * @ref dispatch_inner_loop_strides: @p kernel is instantiated for up to `3^N`
 * stride combinations, where `N` is the number of operands.
 *
 * @see dispatch_inner_loop_strides
 * @see run_elementwise_outer_loop
 */
template <typename Kernel, typename... Pointers>
void run_elementwise_loop(
	const Kernel &kernel,
	const joint_layout &layout,
	Pointers... pointers
);

} // namespace cpu
} // namespace xmipp4

#include "elementwise_loop.inl"
