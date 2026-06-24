// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>

#include <xmipp4/core/span.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_operation_shape_policy
	: public operation_shape_policy
{
public:
	MAKE_CONST_MOCK2(
		deduce,
		void (
			span<shape_type> output_shapes,
			span<const shape_type> input_shapes
		),
		override
	);
};

} // namespace multidimensional
} // namespace xmipp4
