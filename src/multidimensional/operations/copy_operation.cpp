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
	if (input_descriptors.size() != INPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"copy_operation requires exactly one input operand."
		);
	}
	if (output_descriptors.size() != OUTPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"copy_operation requires exactly one output operand."
		);
	}

	array_descriptor &source_desc = 
		input_descriptors[INPUT_OPERAND_SOURCE];
	if (!is_initialized(source_desc))
	{
		throw std::invalid_argument(
			"copy_operation requires source descriptor to be initialized."
		);
	}

	array_descriptor &destination_desc = 
		output_descriptors[OUTPUT_OPERAND_DESTINATION];
	if (is_initialized(destination_desc))
	{
		// Output is initialized, broadcast input to output shape
		std::vector<std::size_t> destination_extents;
		destination_desc.get_layout().get_extents(destination_extents);

		source_desc = array_descriptor(
			source_desc.get_layout().broadcast_to(make_span(destination_extents)),
			source_desc.get_data_type()
		);
	}
	else
	{
		std::vector<std::size_t> source_extents;
		source_desc.get_layout().get_extents(source_extents);

		destination_desc = array_descriptor(
			strided_layout::make_contiguous_layout(make_span(source_extents)),
			source_desc.get_data_type()
		);
	}
}

} // namespace multidimensional
} // namespace xmipp4
