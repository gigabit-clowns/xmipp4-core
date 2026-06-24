// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_data_type_policy.hpp>

#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_operation_data_type_policy
	: public operation_data_type_policy
{
public:
	MAKE_CONST_MOCK2(
		deduce,
		void (
			span<numerical_type> output_types,
			span<const numerical_type> input_types
		),
		override
	);
};

} // namespace multidimensional
} // namespace xmipp4
