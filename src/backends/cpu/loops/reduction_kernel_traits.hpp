// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
 * @defgroup cpu_reduction_kernel_traits Reduction kernel traits
 *
 * What a reduction kernel says about itself, and what the loops size from it.
 *
 * These sit below both the traversal and the adaptor that vectorizes an
 * element kernel, so that neither has to know the other to ask a kernel how
 * much it accumulates or what it will tolerate.
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
 * @brief Number of sub-accumulators one run is dealt out over.
 *
 * A run folded into a single accumulator is a chain of dependent operations,
 * because a fold over inexact arithmetic is not associative and no compiler
 * may break it. Lanes break it deliberately: the run is dealt out over this
 * many sub-accumulators, which are merged once the run is over.
 *
 * Sized from a byte budget the same way the tile is, so a kernel keeping
 * several accumulators takes proportionally fewer lanes. See config.hpp for
 * what the bounds are worth and why.
 *
 * @tparam Accumulators A type_list of accumulator types.
 */
template <typename Accumulators>
struct reduction_fold_lane_count;

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
 * @brief Check whether a kernel's fold may be dealt out over lanes.
 *
 * A kernel says so by declaring
 * @code
 * static constexpr bool reassociable_fold = true;
 * @endcode
 * A kernel that declares it `false`, or does not declare it at all, has its
 * folds kept in order. A `bool` rather than a tag type because the
 * declaration is what a kernel author reads, and it should say what it means
 * without knowing this file.
 *
 * Declaring it states two things: that its `merge` is associative, so the
 * order the lanes are folded in does not matter, and that it is commutative
 * enough for a lane to hold elements that were not next to each other.
 *
 * The second is the one that is easy to lose. A kernel reporting *where* it
 * found something relies on `merge` seeing partial answers in the order the
 * elements were visited, which is what lets it keep the earliest of two equal
 * ones. Dealing every other element to a different lane breaks that, so such
 * a kernel must not declare this. A fold that answers with a value and not a
 * place, which is every operation built on @ref fold_reduction_kernel, may.
 *
 * Declaring it accepts that a fold over inexact arithmetic stops being bit
 * for bit what a single accumulator arrives at. That is the same trade the
 * threaded fold split already makes, and it is what buys an order of
 * magnitude on a long run.
 *
 * @tparam Kernel The reduction kernel.
 */
template <typename Kernel, typename = void>
struct has_reassociable_fold;

/** @} */

namespace detail
{

/**
 * @brief One fixed size array per accumulator type.
 *
 * Separate arrays rather than one array of a composite accumulator: the loops
 * walk the accumulators of consecutive outputs, or of consecutive lanes, so
 * keeping each accumulator contiguous is what lets those walks vectorize.
 *
 * @tparam Accumulators A type_list of accumulator types.
 * @tparam Size How many of each to hold.
 */
template <typename Accumulators, std::size_t Size>
struct accumulator_tiles;

} // namespace detail

} // namespace cpu
} // namespace xmipp4

#include "reduction_kernel_traits.inl"
