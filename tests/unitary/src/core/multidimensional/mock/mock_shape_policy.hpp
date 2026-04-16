// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_shape_policy
	: public shape_policy
{
public:
	MAKE_CONST_MOCK2(
		infer_output, 
		void(
			span<strided_layout> output_layouts,
			span<strided_layout> input_layouts
		),
		override
	);
	MAKE_CONST_MOCK2(
		validate, 
		void(
			span<const strided_layout> output_layouts,
			span<strided_layout> input_layouts
		),
		override
	);
};

} // namespace multidimensional
} // namespace xmipp4
