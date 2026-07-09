// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/layout/access_layout.hpp>
#include <xmipp4/core/layout/access_iterator.hpp>
#include <xmipp4/core/platform/cpp_attributes.hpp>

#include <cstddef>

namespace xmipp4
{
namespace dispatch
{

/**
 * @brief Apply an inner loop to every 1D vector of a multidimensional layout.
 *
 * Iterates over the outer dimensions of the layout, invoking the inner loop
 * once per contiguous 1D vector. On each invocation the operand pointers are
 * advanced to the start of the current vector and the number of elements in it
 * is provided.
 *
 * @tparam InnerLoop Functor to be dispatched for 1D vectors. Must have a
 * signature accepting `(Pointers... operands, std::size_t count)`.
 * @tparam Pointers CV-qualified pointers. There must be one per operand in the
 * layout.
 * @param inner_loop The functor to be invoked for each 1D vector.
 * @param layout Access layout used for iterating over the operands.
 * @param pointers The base pointer of each operand.
 */
template <typename InnerLoop, typename... Pointers>
void run_elementwise_outer_loop(
	InnerLoop &&inner_loop,
	const layout::access_layout &layout,
	Pointers... pointers
);

} // namespace dispatch
} // namespace xmipp4

#include "elementwise_outer_loop.inl"
