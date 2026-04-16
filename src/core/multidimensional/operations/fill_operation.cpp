// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/fill_operation.hpp>

#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/data_type_policies/same_data_type_policy.hpp>
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
			const auto value = fill_value.get<type>();
			const auto *value_start = 
				reinterpret_cast<const std::uint8_t*>(&value);
			const auto *value_end = value_start + sizeof(type);

			return fmt::format(
				"{}({:02x})", 
				type_str, 
				fmt::join(value_start, value_end, "")
			);
		},
		native_type_map(),
		data_type
	);
}

const shape_policy& fill_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& fill_operation::get_data_type_policy() const noexcept
{
	return same_data_type_policy::get();
}

std::size_t fill_operation::get_output_count() const noexcept { return 1; }
std::size_t fill_operation::get_input_count() const noexcept { return 0; }

scalar_ref fill_operation::get_fill_value() const noexcept
{
	return m_fill_value;
}

} // namespace multidimensional
} // namespace xmipp4
