// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/layout/joint_layout.hpp>
#include <xmipp4/core/span.hpp>

#include <array>
#include <cstddef>

namespace xmipp4
{
namespace cpu
{

/**
 * @brief Splits a matmul/matvec/vecmat/cross operand into a batch and a
 * core.
 *
 * Every one of these operations walks broadcastable "batch" axes plus a
 * small, fixed number of "core" axes per operand that a single kernel call
 * consumes whole (two for a matrix operand, one for a vector operand or for
 * cross's named axis). This class performs that split once, up front, so the
 * program builder only has to drive an ordinary elementwise-shaped loop over
 * the batch and hand the core, raw, to the kernel.
 *
 * @see run_elementwise_loop
 */
class linalg_core_layout_plan
{
public:
	/**
	 * @brief The raw, un-reordered core axes of one operand.
	 *
	 * Never merged into the batch joint_layout: the kernel receives it
	 * directly and decides what to do with it.
	 */
	struct operand_core
	{
		std::size_t rank = 0;
		std::array<std::size_t, 2> extents{};
		std::array<std::ptrdiff_t, 2> strides{};
	};

	/**
	 * @brief Plan operands whose core axes are their trailing axes.
	 *
	 * The output's own extents, minus its trailing @p output_core_rank
	 * axes, are trusted as the canonical batch shape: the shape policy
	 * already broadcast and validated it when it deduced the output shape.
	 * Every operand's remaining batch prefix is then handed to a
	 * joint_layout_builder, which broadcasts it to that shape.
	 *
	 * @param output_signatures Exactly one output signature.
	 * @param input_signatures Exactly two input signatures (left, right).
	 * @param output_core_rank Number of trailing axes of the output that
	 * are core.
	 * @param left_core_rank Number of trailing axes of `left` that are
	 * core.
	 * @param right_core_rank Number of trailing axes of `right` that are
	 * core.
	 *
	 * @throws std::invalid_argument If an operand's rank is smaller than
	 * its declared core rank.
	 */
	static linalg_core_layout_plan for_trailing_core(
		span<const operand_signature> output_signatures,
		span<const operand_signature> input_signatures,
		std::size_t output_core_rank,
		std::size_t left_core_rank,
		std::size_t right_core_rank
	);

	/**
	 * @brief Plan operands sharing one named core axis (cross).
	 *
	 * Every operand's full layout is broadcast to the output's full
	 * extents first (an ordinary elementwise-style broadcast, since the
	 * named axis is a broadcast axis pinned to a fixed extent, not an
	 * asymmetric contraction), and the axis at @p core_axis is then pulled
	 * out of the broadcast result as the core.
	 *
	 * @param core_axis Index into the canonical (post-broadcast) rank,
	 * the same index for every operand.
	 */
	static linalg_core_layout_plan for_named_axis(
		span<const operand_signature> output_signatures,
		span<const operand_signature> input_signatures,
		std::size_t core_axis
	);

	/**
	 * @brief The batch iteration layout.
	 *
	 * Operands in order: output, left, right.
	 *
	 * @return const joint_layout& The batch layout.
	 */
	const joint_layout& get_batch_layout() const noexcept;

	const operand_core& get_output_core() const noexcept;
	const operand_core& get_left_core() const noexcept;
	const operand_core& get_right_core() const noexcept;

private:
	linalg_core_layout_plan(
		joint_layout batch_layout,
		operand_core output_core,
		operand_core left_core,
		operand_core right_core
	) noexcept;

	joint_layout m_batch_layout;
	operand_core m_output_core;
	operand_core m_left_core;
	operand_core m_right_core;
};

} // namespace cpu
} // namespace xmipp4
