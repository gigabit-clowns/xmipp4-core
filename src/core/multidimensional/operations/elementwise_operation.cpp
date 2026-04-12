// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/elementwise_operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>

namespace xmipp4 
{
namespace multidimensional
{

void elementwise_operation::sanitize_operands(
	span<array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	for (const auto &input_descriptor : input_descriptors)
	{
		if (!is_initialized(input_descriptor))
		{
			throw std::invalid_argument(
				"elementwise_operation requires input descriptor to be "
				"initialized."
			);
		}
	}

	if (is_initialized(output_descriptors[0]))
	{
		broadcast_input_to_output(output_descriptors, input_descriptors);
	}
	else
	{
		deduce_output(output_descriptors, input_descriptors);
	}
}

void elementwise_operation::broadcast_input_to_output(
	span<const array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	std::vector<std::size_t> output_extents;
	output_descriptors[0].get_layout().get_extents(output_extents);

	for (std::size_t i = 1; i < output_descriptors.size(); ++i)
	{
		const auto &layout = output_descriptors[i].get_layout();
		if(!layout.extents_equal(make_span(output_extents)))
		{
			throw std::invalid_argument(
				"multi-output elementwise_operation requires all outputs to"
				"have the same extents."
			);
		}
	}

	for (std::size_t i = 0; i < input_descriptors.size(); ++i)
	{
		auto &input_desc = input_descriptors[i];
		input_desc = array_descriptor(
			input_desc.get_layout().broadcast_to(make_span(output_extents)),
			input_desc.get_data_type()
		);
	}
}

void elementwise_operation::deduce_output(
	span<array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	std::vector<std::size_t> broadcasted_extents;
	input_descriptors[0].get_layout().get_extents(broadcasted_extents);

	std::vector<std::size_t> extents;
	for (std::size_t i = 1; i < input_descriptors.size(); ++i)
	{
		input_descriptors[0].get_layout().get_extents(extents);
		broadcast_extents(broadcasted_extents, extents);
	}

	for (std::size_t i = 0; i < output_descriptors.size(); ++i)
	{
		output_descriptors[i] = array_descriptor(
			strided_layout::make_contiguous_layout(
				make_span(broadcasted_extents)
			),
			numerical_type::unknown // Set by child classes.
		);
	}

	for (std::size_t i = 0; i < input_descriptors.size(); ++i)
	{
		auto &input_desc = input_descriptors[i];
		input_desc = array_descriptor(
			input_desc.get_layout().broadcast_to(make_span(broadcasted_extents)),
			input_desc.get_data_type()
		);
	}
}

void unary_elementwise_operation::sanitize_operands(
	span<array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	if (output_descriptors.size() != 1)
	{
		throw std::invalid_argument(
			"unary_operation requires a single output operand"
		);
	}

	if (input_descriptors.size() != 1)
	{
		throw std::invalid_argument(
			"unary_operation requires a single input operand"
		);
	}

	operation::sanitize_operands(output_descriptors, input_descriptors);
}

void binary_elementwise_operation::sanitize_operands(
	span<array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	if (output_descriptors.size() != 1)
	{
		throw std::invalid_argument(
			"binary_operation requires a single output operand"
		);
	}

	if (input_descriptors.size() != 2)
	{
		throw std::invalid_argument(
			"binary_operation requires two input operands"
		);
	}

	operation::sanitize_operands(output_descriptors, input_descriptors);
}

void ternary_elementwise_operation::sanitize_operands(
	span<array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	if (output_descriptors.size() != 1)
	{
		throw std::invalid_argument(
			"ternary_operation requires a single output operand"
		);
	}

	if (input_descriptors.size() != 3)
	{
		throw std::invalid_argument(
			"binary_operation requires three input operands"
		);
	}

	operation::sanitize_operands(output_descriptors, input_descriptors);
}

} // namespace multidimensional
} // namespace xmipp4
