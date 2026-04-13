// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/compare_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

void compare_operation::sanitize_operands(
	span<array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	binary_elementwise_operation::sanitize_operands(
		output_descriptors, 
		input_descriptors
	);
	// TODO: Ensure output types are equal and ensure output type is bool.
}

std::string less_operation::get_name() const
{
	return "less";
}

std::string less_equal_operation::get_name() const
{
	return "less_equal";
}

std::string equal_operation::get_name() const
{
	return "equal";
}

std::string inequal_operation::get_name() const
{
	return "inequal";
}

} // namespace multidimensional
} // namespace xmipp4
