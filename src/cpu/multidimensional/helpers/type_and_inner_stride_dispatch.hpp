// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/layout/access_layout.hpp>

#include <cstddef>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Compose numerical-type and inner-loop-stride dispatch for a CPU
 * operation program builder.
 *
 * Resolves each of the runtime data types to a native C++ type and each
 * operand's runtime stride to a stride tag, then invokes @p callable exactly
 * once with the fully static combination. This is the dispatch staircase
 * shared by every CPU operation program builder; only the number of type
 * axes, the operand count and the final maker differ between operations.
 *
 * The callable is invoked as:
 * @code
 * callable(std::move(access_layout), type_list<Ts...>(), std::tuple<Strides...>())
 * @endcode
 * where `Ts...` are the resolved native types (one per @p data_types entry,
 * `void` if a type fails to resolve) and the tuple holds one stride tag per
 * operand. @p access_layout is moved into @p callable at the innermost
 * dispatch level so the callable can sink it into the resulting program.
 *
 * @tparam NOperands Number of operands whose strides are dispatched.
 * @tparam F Type of the callable to be invoked.
 * @tparam DataTypes Pack of `numerical_type` values to resolve.
 * @param callable Functor invoked with the resolved static combination.
 * @param access_layout Layout to dispatch strides from and move into
 * @p callable.
 * @param data_types Runtime numerical types to resolve, one per type axis.
 * @return auto The result of invoking @p callable.
 *
 * @see dispatch_numerical_types
 * @see dispatch_inner_loop_strides
 */
template <std::size_t NOperands, typename F, typename... DataTypes>
auto dispatch_types_and_inner_strides(
	F &&callable,
	layout::access_layout access_layout,
	DataTypes... data_types
);

} // namespace multidimensional
} // namespace xmipp4

#include "type_and_inner_stride_dispatch.inl"
