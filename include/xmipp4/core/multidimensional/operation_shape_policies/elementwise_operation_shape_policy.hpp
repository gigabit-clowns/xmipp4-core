// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Shape policy for elementwise operations.
 *
 * Unlike `homogeneous_operation_shape_policy` this policy allows broadcast
 * compatibility of input operands.
 */
class XMIPP4_CORE_API elementwise_operation_shape_policy final
	: public operation_shape_policy
{
public:
	void deduce_output(
		span<shape_type> output_shapes,
		span<const shape_type> input_shapes
	) const override;

	void validate(
		span<const shape_type> output_shapes,
		span<const shape_type> input_shapes
	) const override;

	static const elementwise_operation_shape_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4

