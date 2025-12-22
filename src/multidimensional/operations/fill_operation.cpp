// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/fill_operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>

namespace xmipp4 
{
namespace multidimensional
{

fill_operation::fill_operation() noexcept = default;
fill_operation::~fill_operation()  = default;

std::string fill_operation::get_name() const
{
	return "fill";
}

std::string fill_operation::serialize_parameters() const
{
	// TODO serialize fill value
	return "";
}

void fill_operation::sanitize_operands(
	span<array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	if (input_descriptors.size() != 0)
	{
		throw std::invalid_argument(
			"fill_operation requires exactly no input operand."
		);
	}
	if (output_descriptors.size() != 1)
	{
		throw std::invalid_argument(
			"fill_operation requires exactly one output operand."
		);
	}

	array_descriptor &output_desc = output_descriptors[0];
	if (output_desc.get_data_type() == numerical_type::unknown) // TODO: Ensure equal to fill value type
	{
		throw std::invalid_argument(
			"Output array data type must be the same as the fill value type."
		);
	}
}

} // namespace multidimensional
} // namespace xmipp4
