// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/span.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <vector>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Strategy interface that deduces the shapes of an operation's output 
 * arrays from its input shapes.
 *
 * @see operation_data_type_policy
 */
class XMIPP4_CORE_API operation_shape_policy
{
public:
	/// Sequence of extents, one per dimension.
	using shape_type = std::vector<std::size_t>;

	operation_shape_policy() noexcept;
	operation_shape_policy(const operation_shape_policy &other) = delete;
	operation_shape_policy(operation_shape_policy &&other) = delete;
	virtual ~operation_shape_policy();

	operation_shape_policy&
	operator=(const operation_shape_policy &other) = delete;
	operation_shape_policy&
	operator=(operation_shape_policy &&other) = delete;

	/**
	 * @brief Deduce the shapes of the output tensors.
	 *
	 * Fills each element of @p output_shapes with the @ref shape_type
	 * that the operation would produce for the corresponding output tensor,
	 * given the shapes of all input tensors in @p input_shapes.
	 *
	 * @param output_shapes Writable span whose length equals the number of
	 * output tensors; each entry is set to the deduced output shape.
	 * @param input_shapes Read-only span whose length equals the number of
	 * input tensors; each entry holds the shape of the corresponding input
	 * tensor.
	 */
	virtual void deduce(
		span<shape_type> output_shapes,
		span<const shape_type> input_shapes
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
