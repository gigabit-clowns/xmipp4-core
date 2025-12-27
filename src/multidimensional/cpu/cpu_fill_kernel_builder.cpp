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

namespace
{

template <typename T, typename Q>
std::shared_ptr<cpu_kernel> make_fill_kernel(
	array_access_layout access_layout,
	const scalar_ref &fill_value,
	std::true_type
)
{
	const auto value = fill_value.get<Q>();

	return std::make_shared<cpu_fill_kernel<T>>(
		std::move(access_layout), 
		static_cast<T>(value)
	);
}

template <typename T, typename Q>
std::shared_ptr<cpu_kernel> make_fill_kernel(
	array_access_layout,
	const scalar_ref&,
	std::false_type
)
{
	throw std::invalid_argument(
		"cpu_fill_kernel_builder::build: Fill value can not be converted into "
		"the output value"
	);
}

} // anonymous namespace

operation_id 
cpu_fill_kernel_builder::get_operation_id() const noexcept
{
	return operation_id::of<fill_operation>();
}

backend_priority cpu_fill_kernel_builder::get_suitability(
	const operation& operation,
	span<const array_descriptor>,
	hardware::device &device
) const
{
	if (dynamic_cast<const fill_operation*>(&operation) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel_builder::build: Expected operation to be an "
			"instance of fill_operation"
		);
	}

	if (dynamic_cast<const hardware::cpu_device*>(&device) != nullptr)
	{
		return backend_priority::normal;
	}

	return backend_priority::unsupported;
}

std::shared_ptr<kernel> cpu_fill_kernel_builder::build(
	const operation& operation,
	span<const array_descriptor> descriptors,
	hardware::device& device
) const
{
	if (dynamic_cast<const fill_operation*>(&operation) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel_builder::build: Expected operation to be an "
			"instance of fill_operation"
		);
	}

	if (dynamic_cast<const hardware::cpu_device*>(&device) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel_builder::build: Expected device to be an instance "
			"of cpu_device"
		);
	}

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
			
			return make_fill_kernel<output_value_type, fill_value_type>(
				std::move(access_layout),
				fill_value,
				typename std::is_convertible<
					fill_value_type, 
					output_value_type
				>::type()
			);
		},
		output_descriptor.get_data_type(),
		fill_value.get_data_type()
	);
}

} // namespace multidimensional
} // namespace xmipp4
