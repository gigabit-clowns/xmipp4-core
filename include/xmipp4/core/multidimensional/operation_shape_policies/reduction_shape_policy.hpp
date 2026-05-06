// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <vector>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Shape policy for reduction operations.
 *
 * Inputs are first broadcast together to form the reduction space. The
 * reduction axes are then applied to that space to produce a single
 * canonical output shape, which is written to all output slots.
 *
 * An empty axis list means reduce over every dimension (global reduction).
 * When @p keep_dim is true, reduced dimensions are retained in the output
 * with extent 1 instead of being dropped, preserving the rank for
 * subsequent broadcast operations.
 */
class reduction_operation_shape_policy final
	: public operation_shape_policy
{
public:
	/**
	 * @brief Construct from a const-ref axes list.
	 *
	 * @param axes Axes to reduce along. Duplicates are silently removed and
	 * the remaining axes are sorted in ascending order. An empty list
	 * means reduce over all dimensions.
	 * @param keep_dim When true, reduced dimensions are kept in the output
	 * with extent 1 instead of being removed.
	 */
	XMIPP4_CORE_API
	explicit reduction_operation_shape_policy(
		const std::vector<std::size_t> &axes,
		bool keep_dim = false
	);

	/**
	 * @brief Construct from an rvalue axes list.
	 *
	 * @param axes Axes to reduce along. Duplicates are silently removed and
	 * the remaining axes are sorted in ascending order. An empty list
	 * means reduce over all dimensions.
	 * @param keep_dim When true, reduced dimensions are kept in the output
	 * with extent 1 instead of being removed.
	 */
	XMIPP4_CORE_API
	explicit reduction_operation_shape_policy(
		std::vector<std::size_t> &&axes,
		bool keep_dim = false
	);

	reduction_operation_shape_policy(
		const reduction_operation_shape_policy &other
	) = delete;

	reduction_operation_shape_policy(
		reduction_operation_shape_policy &&other
	) noexcept = delete;

	XMIPP4_CORE_API
	~reduction_operation_shape_policy() override;

	reduction_operation_shape_policy&
	operator=(const reduction_operation_shape_policy &other) = delete;
	reduction_operation_shape_policy&
	operator=(reduction_operation_shape_policy &&other) noexcept = delete;

	XMIPP4_CORE_API
	void deduce(
		span<shape_type> canonical_output_shapes,
		span<const shape_type> input_shapes
	) const override;

	/**
	 * @brief Return the sorted, deduplicated reduction axes.
	 *
	 * @return A non-owning view of the internal axes vector. An empty span
	 * indicates a reduce-all policy.
	 */
	XMIPP4_CORE_API
	span<const std::size_t> get_reduction_axes() const noexcept;

	/**
	 * @brief Return whether reduced dimensions are kept in the output.
	 *
	 * @return true if reduced dimensions are retained with extent 1;
	 * false if they are removed from the output shape.
	 */
	XMIPP4_CORE_API
	bool keeps_dimensions() const noexcept;

private:
	std::vector<std::size_t> m_reduction_axes; ///< Sorted and deduplicated.
	bool m_keep_dimensions;
};

} // namespace multidimensional
} // namespace xmipp4
