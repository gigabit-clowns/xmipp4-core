// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Shape policy to ensure that all operands have the same shape.
 */
class XMIPP4_CORE_API homogeneous_operation_shape_policy final
	: public operation_shape_policy
{
public:
	void deduce(
		span<shape_type> canonical_output_shapes,
		span<const shape_type> input_shapes
	) const override;

	static const homogeneous_operation_shape_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4

