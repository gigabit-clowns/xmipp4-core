// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/fill_operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>

#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

fill_operation::fill_operation(scalar_ref fill_value) noexcept
	: m_fill_value(fill_value)
{
}

fill_operation::~fill_operation()  = default;

std::string fill_operation::get_name() const
{
	return "fill";
}

std::string fill_operation::serialize_parameters() const
{
	std::ostringstream oss;
	oss << to_string(m_fill_value.get_data_type()) << "(";

	const auto &fill_value = m_fill_value;
	visit(
		[&oss, &fill_value] (auto tag)
		{
			using type = typename decltype(tag)::type;
			oss << fill_value.get<type>();
		},
		fill_value.get_data_type()
	);

	oss << ")";

	return oss.str();
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
	if (is_initialized(output_desc))
	{
		throw std::invalid_argument(
			"Output array must be initialized."
		);
	}
}

} // namespace multidimensional
} // namespace xmipp4
