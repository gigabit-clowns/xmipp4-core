// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>

namespace xmipp4 
{
namespace multidimensional
{

copy_operation::copy_operation() noexcept = default;
copy_operation::~copy_operation()  = default;

std::string copy_operation::get_name() const
{
	return "copy";
}

void copy_operation::sanitize_operands(
	span<array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	if (input_descriptors.size() != 1)
	{
		throw std::invalid_argument(
			"copy_operation requires exactly one input operand."
		);
	}
	if (output_descriptors.size() != 1)
	{
		throw std::invalid_argument(
			"copy_operation requires exactly one output operand."
		);
	}

	array_descriptor &input_desc = input_descriptors[0];
	if (!is_initialized(input_desc))
	{
		throw std::invalid_argument(
			"copy_operation requires input descriptor to be initialized."
		);
	}

	array_descriptor &output_desc = output_descriptors[0];
	if (is_initialized(output_desc))
	{
		// TODO check if input is convertible to output type.

		// Output is initialized, broadcast input to output shape
		std::vector<std::size_t> output_extents;
		output_desc.get_layout().get_extents(output_extents);

		input_desc = array_descriptor(
			input_desc.get_layout().broadcast_to(make_span(output_extents)),
			input_desc.get_data_type()
		);
	}
	else
	{
		output_desc = input_desc;
	}
}

} // namespace multidimensional
} // namespace xmipp4
