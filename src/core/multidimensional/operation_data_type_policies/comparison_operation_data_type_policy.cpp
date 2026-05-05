// SPDX-License-Identifier: GPL-3.0-only

#include "comparison_operation_data_type_policy.hpp"

namespace xmipp4
{
namespace multidimensional
{

void comparison_operation_data_type_policy::deduce(
	span<numerical_type> canonical_output_types,
	span<const numerical_type> input_types
) const
{
	XMIPP4_ASSERT(canonical_output_types.size() == 1);
	XMIPP4_ASSERT(input_types.size() == 2);

	if (input_types[0] != input_types[1])
	{
		throw std::invalid_argument(
			"comparison_operation_data_type_policy::deduce: expected equal "
			"input operands."
		);
	}

	const auto reference = input_types[0];
	if (get_size(reference) == 0)
	{
		throw std::invalid_argument(
			"comparison_operation_data_type_policy::deduce: unknown input "
			"type."
		);
	}

	if (get_category(reference) == numerical_type_category::complex)
	{
		throw std::invalid_argument(
			"comparison_operation_data_type_policy::deduce: can not operate "
			"on complex types."
		);
	}

	canonical_output_types[0] = numerical_type::boolean;
}

const comparison_operation_data_type_policy& 
comparison_operation_data_type_policy::get() noexcept
{
	static const comparison_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
