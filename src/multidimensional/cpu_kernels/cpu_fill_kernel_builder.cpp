// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_fill_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/fill_operation.hpp>
#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/cpu/cpu_device.hpp>

#include "cpu_fill_kernel.hpp"

namespace xmipp4 
{
namespace multidimensional
{

operation_id 
cpu_fill_kernel_builder::get_operation_id() const noexcept
{
	return operation_id::of<fill_operation>();
}

backend_priority cpu_fill_kernel_builder::get_suitability(
	const operation&,
	span<const array_descriptor>,
	hardware::device &device
) const
{
	if (dynamic_cast<const hardware::cpu_device*>(&device) != nullptr)
	{
		return backend_priority::normal;
	}

	return backend_priority::unsupported;
}

std::shared_ptr<kernel> cpu_fill_kernel_builder::build(
	const operation& operation,
	span<const array_descriptor> descriptors,
	hardware::device&
) const
{
	if (descriptors.size() != 1)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel_builder::build: Expected exactly 1 "
			"array descriptor."
		);
	}

	const auto &output_descriptor = descriptors[0];

	array_access_layout_builder layout_builder;
	layout_builder.add_operand(output_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	const auto fill_value = 
		dynamic_cast<const fill_operation&>(operation).get_fill_value();

	return visit(
		[&access_layout, &fill_value] 
		(auto output_tag, auto fill_tag) -> std::shared_ptr<cpu_kernel>
		{
			using output_value_type = typename decltype(output_tag)::type;
			using fill_value_type = typename decltype(fill_tag)::type;
			using kernel_type = 
				cpu_fill_kernel<output_value_type, fill_value_type>;

			const auto value = fill_value.get<fill_value_type>();

			return std::make_shared<kernel_type>(
				std::move(access_layout), 
				value
			);
		},
		output_descriptor.get_data_type(),
		fill_value.get_data_type()
	);
}

} // namespace multidimensional
} // namespace xmipp4
