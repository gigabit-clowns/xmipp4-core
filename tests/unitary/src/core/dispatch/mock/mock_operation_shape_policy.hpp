// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/operation_shape_policy.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{

class mock_operation_shape_policy
	: public operation_shape_policy
{
public:
	using shape_type = operation_shape_policy::shape_type;

	MAKE_CONST_MOCK2(
		deduce,
		void(
			span<shape_type> canonical_output_shapes,
			span<const shape_type> input_shapes
		),
		override
	);
	MAKE_CONST_MOCK3(
		accept,
		void(
			span<const shape_type> user_output_shapes,
			span<const shape_type> canonical_output_shapes,
			span<const shape_type> input_shapes
		),
		override
	);
};

} // namespace xmipp4
