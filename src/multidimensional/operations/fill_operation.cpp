// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/fill_operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>

#include <fmt/format.h>
#include <fmt/ranges.h>

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
	const auto &fill_value = m_fill_value;
	const auto data_type = fill_value.get_data_type();
	const auto *type_str = to_string(data_type);

	return dispatch_numerical_types(
		[type_str, &fill_value] (auto tag)
		{
			using type = typename decltype(tag)::type;
			const auto *value_start = reinterpret_cast<const std::uint8_t*>(
				&fill_value.get<type>()
			);
			const auto *value_end = value_start + sizeof(type);

			return fmt::format(
				"{}({:02x})", 
				type_str, 
				fmt::join(value_start, value_end, "")
    		);
		},
		data_type
	);
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
	if (!is_initialized(output_desc))
	{
		throw std::invalid_argument(
			"Output array must be initialized."
		);
	}

	// TODO check if fill value is convertible to output type.
}

scalar_ref fill_operation::get_fill_value() const noexcept
{
	return m_fill_value;
}

} // namespace multidimensional
} // namespace xmipp4
