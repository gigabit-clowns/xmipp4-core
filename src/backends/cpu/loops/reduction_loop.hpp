// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/layout/joint_layout.hpp>
#include <xmipp4/core/meta/type_list.hpp>

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>

namespace xmipp4
{
namespace cpu
{

/**
 * @defgroup cpu_reduction_loop Reduction loop
 *
 * The iteration scheme shared by every CPU reduction program builder.
 *
 * A reduction folds the elements of its inputs along a set of axes. The axes
 * are a parameter of the operation, so the partition of the iteration space
 * into reduced and kept axes is known when the program is built and is
 * expressed as **two layouts**: one over the axes that survive into the
 * output and one over the axes being folded away.
 *
 * Splitting the space in two, rather than using a single layout with a
 * broadcast output, is what makes the loop nest expressible. A single layout
 * sorts its axes by memory locality, which interleaves reduced and kept axes
 * in an order the operation does not control, and the traversal API only
 * slices a contiguous range of axes. Two layouts also let each half be sorted
 * and coalesced for its own traversal instead of sharing one compromise
 * ordering.
 *
 * @{
 */

/**
 * @brief Total size in bytes of a list of accumulator types.
 *
 * @tparam Accumulators A type_list of accumulator types.
 */
template <typename Accumulators>
struct accumulator_footprint;

/**
 * @brief Number of outputs a reduction completes at a time.
 *
 * The loop holds one accumulator per output being worked on, so the tile is
 * sized to keep the accumulators resident in the first level cache while the
 * inputs stream past. A kernel declaring several accumulators therefore takes
 * a proportionally shorter tile.
 *
 * The bounds keep the tile long enough for the input reads to stream even
 * when the accumulators are large, and short enough to stay resident even
 * when they are small.
 *
 * @tparam Accumulators A type_list of accumulator types.
 */
template <typename Accumulators>
struct reduction_tile_size;

/**
 * @brief Check whether a kernel supplies an identity.
 *
 * An identity is only needed to answer a reduction over no elements at all.
 * Kernels whose fold has no neutral element, such as the location of an
 * extremum, do not provide one and reject that case instead.
 *
 * @tparam Kernel The reduction kernel.
 * @tparam Accumulators A type_list of its accumulator types.
 */
template <typename Kernel, typename Accumulators>
struct has_reduction_identity;

/**
 * @brief Run a reduction over a layout, folding the reduced axes into an
 * accumulator per output element.
 *
 * The nest walks @p kept_layout at 1D-vector granularity, blocks each vector
 * into tiles, and for every tile folds the whole of @p reduced_layout into
 * stack local accumulators before writing the outputs once. Completing a
 * tile before moving on is what keeps the accumulators in registers or in
 * cache, removes the read-modify-write an accumulate-through-the-output
 * scheme would perform on every element, and lets the accumulator type and
 * count be unrelated to those of the outputs.
 *
 * The accumulators are seeded from the first element rather than from an
 * identity. A fold that has no neutral element is therefore expressed the
 * same way as one that has, and a reduction over a single element costs
 * nothing beyond reading it.
 *
 * @p kernel is invoked as:
 * @code
 * kernel.seed    (accumulators..., inputs..., position)
 * kernel.combine (accumulators..., inputs..., position)
 * kernel.finalize(outputs..., accumulators..., count)
 * kernel.identity(accumulators...)               // only when count is zero
 * @endcode
 * where `accumulators` are mutable references, `inputs` are pointers to the
 * element of each input for the current iteration and `outputs` are pointers
 * to the element of each output being completed. `position` says where in
 * the reduced space the element sits, counted from the first element folded
 * into that output; only an operation reporting a location has any use for
 * it, and the arithmetic behind it disappears for the operations that
 * ignore it. It is only meaningful when the reduced space is traversed in a
 * defined order, which @ref reduction_layout_plan arranges on request.
 *
 * The accumulator types come from `Kernel::accumulators<Outputs, Inputs>::type`
 * and are unrelated to the number of outputs: a kernel may keep two
 * accumulators and write one output, or the other way round.
 *
 * A kernel additionally carries
 * @code
 * kernel.merge(accumulators..., others...)
 * @endcode
 * folding one set of accumulators into another. This loop never calls it,
 * being single threaded: it is the part of the concept a parallel reduction
 * needs, where a reduced vector is split across workers and their partial
 * accumulators are combined at the end. It is reserved for that rather than
 * left over from anything, and is kept in step with `combine` so the two
 * cannot disagree once it is used.
 *
 * @tparam Kernel The reduction kernel.
 * @tparam Outs Element types of the outputs.
 * @tparam Ins Element types of the inputs.
 * @param kernel The kernel describing the fold.
 * @param kept_layout Layout over the axes that survive into the output. Its
 * operands are the inputs, in order, followed by the outputs, in order.
 * @param reduced_layout Layout over the axes being folded away. Its operands
 * are the inputs, in order.
 * @param reduction_count Number of elements folded into each output, that is
 * the product of the reduced extents. Passed to `finalize` for the operations
 * whose result depends on it, such as an average.
 * @param outputs Base pointer of each output.
 * @param inputs Base pointer of each input.
 *
 * @throws std::invalid_argument When the reduction is over no elements and
 * @p kernel supplies no identity.
 *
 * @warning Inherits the stride specialization cost of
 * @ref dispatch_inner_loop_strides on both layouts: @p kernel is instantiated
 * for up to `3^(2N)` stride combinations, where `N` is the number of inputs.
 * The outputs are deliberately left out of the dispatch, so the cost does not
 * grow with the number of outputs.
 */
template <typename Kernel, typename... Outs, typename... Ins>
void run_reduction_loop(
	const Kernel &kernel,
	const joint_layout &kept_layout,
	const joint_layout &reduced_layout,
	std::size_t reduction_count,
	const std::tuple<Outs*...> &outputs,
	const std::tuple<const Ins*...> &inputs
);

/** @} */

} // namespace cpu
} // namespace xmipp4

#include "reduction_loop.inl"
