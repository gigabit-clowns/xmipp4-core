// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_data_type_policy
	: public data_type_policy
{
public:
	MAKE_CONST_MOCK2(
		infer_output, 
		void(
			span<numerical_type> output_types,
			span<const numerical_type> input_types
		),
		override
	);
	MAKE_CONST_MOCK2(
		validate, 
		void(
			span<const numerical_type> output_types,
			span<const numerical_type> input_types
		),
		override
	);
};

} // namespace multidimensional
} // namespace xmipp4
