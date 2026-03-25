// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/sum_operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>

#include <algorithm>

#include <fmt/format.h>
#include <fmt/ranges.h>

namespace xmipp4 
{
namespace multidimensional
{

sum_operation::sum_operation(std::vector<std::size_t> axes)
	: m_axes(std::move(axes))
{
	// Keep only unique axes.
	std::sort(m_axes.begin(), m_axes.end());
	m_axes.erase(std::unique(m_axes.begin(), m_axes.end()), m_axes.end());
}

sum_operation::~sum_operation()  = default;

std::string sum_operation::get_name() const
{
	return "sum";
}

std::string sum_operation::serialize_parameters() const
{
	return fmt::format("{}", fmt::join(m_axes.cbegin(), m_axes.cend(), ","));
}

void sum_operation::sanitize_operands(
	span<array_descriptor> output_descriptors,
	span<array_descriptor> input_descriptors
) const
{
	if (input_descriptors.size() != INPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"sum_operation requires exactly one input operands."
		);
	}
	if (output_descriptors.size() != OUTPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"sum_operation requires exactly one output operand."
		);
	}

	for (std::size_t i = 0; i < INPUT_OPERAND_COUNT; ++i)
	{
		if (!is_initialized(input_descriptors[i]))
		{
			throw std::invalid_argument(
				"sum_operation requires input descriptor to be initialized."
			);
		}
	}

	// Expected output layout:
	// extent=1 stride=0 for axis indices in m_axes
	// extent and stride of the input for axis indices *not* in m_axes
	const auto data_type = input_descriptors[0].get_data_type();
	auto &result_desc = output_descriptors[OUTPUT_OPERAND_RESULT];
	const auto &input_desc = input_descriptors[INPUT_OPERAND_X];
	if (is_initialized(result_desc))
	{
		if (result_desc.get_data_type() == data_type)
		{
			throw std::invalid_argument(
				"sum_operation requires output operand to have the same data "
				"type as input operands."
			);
		}

		// TODO check the output is shaped correctly
	}
	else
	{
		result_desc = array_descriptor(
			strided_layout(), // TODO
			data_type
		);
	}
}

} // namespace multidimensional
} // namespace xmipp4
