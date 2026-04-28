// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/span.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <vector>

namespace xmipp4
{
namespace multidimensional
{

class strided_layout;

/**
 * @brief Interface describing the validation and deduction of array shapes.
 */
class XMIPP4_CORE_API operation_shape_policy
{
public:
	using shape_type = std::vector<std::size_t>;

	operation_shape_policy() noexcept;
	operation_shape_policy(const operation_shape_policy &other) = delete;
	operation_shape_policy(operation_shape_policy &&other) = delete;
	virtual ~operation_shape_policy();

	operation_shape_policy& operator=(const operation_shape_policy &other) = delete;
	operation_shape_policy& operator=(operation_shape_policy &&other) = delete;

	virtual
	void deduce_output(
		span<shape_type> output_shapes,
		span<const shape_type> input_shapes
	) const = 0;

	virtual
	void validate(
		span<const shape_type> output_shapes,
		span<const shape_type> input_shapes
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
