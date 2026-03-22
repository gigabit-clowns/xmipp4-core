// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/add_operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>

namespace xmipp4 
{
namespace multidimensional
{

add_operation::add_operation() noexcept = default;
add_operation::~add_operation()  = default;

std::string add_operation::get_name() const
{
	return "add";
}

void add_operation::sanitize_operands(
	span<array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	if (input_descriptors.size() != INPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"add_operation requires exactly two input operands."
		);
	}
	if (output_descriptors.size() != OUTPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"add_operation requires exactly one output operand."
		);
	}

	for (std::size_t i = 0; i < INPUT_OPERAND_COUNT; ++i)
	{
		if (!is_initialized(input_descriptors[i]))
		{
			throw std::invalid_argument(
				"add_operation requires input descriptor to be initialized."
			);
		}
	}

	const auto data_type = input_descriptors[0].get_data_type();
	if (input_descriptors[1].get_data_type() == data_type)
	{
		throw std::invalid_argument(
			"add_operation requires both input operands to have the same "
			"data type"
		);
	}

	array_descriptor &result_desc = output_descriptors[OUTPUT_OPERAND_RESULT];
	if (is_initialized(result_desc))
	{

		if (result_desc.get_data_type() == data_type)
		{
			throw std::invalid_argument(
				"add_operation requires output operand to have the same data "
				"type as input operands."
			);
		}

		// Output is initialized, broadcast input to output shape
		std::vector<std::size_t> result_extents;
		result_desc.get_layout().get_extents(result_extents);

		for (std::size_t i = 0; i < INPUT_OPERAND_COUNT; ++i)
		{
			input_descriptors[i] = array_descriptor(
				input_descriptors[i].get_layout()
					.broadcast_to(make_span(result_extents)),
				input_descriptors[i].get_data_type()
			);
		}
	}
	else
	{
		std::vector<std::size_t> result_extents;
		std::vector<std::size_t> input_extents;
		for (std::size_t i = 0; i < INPUT_OPERAND_COUNT; ++i)
		{
			input_descriptors[i].get_layout().get_extents(input_extents);
			broadcast_extents(result_extents, input_extents);
		}

		for (std::size_t i = 0; i < INPUT_OPERAND_COUNT; ++i)
		{
			input_descriptors[i] = array_descriptor(
				input_descriptors[i].get_layout()
					.broadcast_to(make_span(result_extents)),
				input_descriptors[i].get_data_type()
			);
		}

		result_desc = array_descriptor(
			strided_layout::make_contiguous_layout(make_span(result_extents)),
			data_type
		);
	}
}

} // namespace multidimensional
} // namespace xmipp4
