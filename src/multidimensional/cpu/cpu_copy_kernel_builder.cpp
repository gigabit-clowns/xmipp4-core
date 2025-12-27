// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>
#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/cpu/cpu_device.hpp>

#include "cpu_copy_kernel.hpp"

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

template <typename T, typename Q>
std::shared_ptr<cpu_kernel> make_copy_kernel(
	array_access_layout access_layout,
	std::true_type
)
{
	return std::make_shared<cpu_copy_kernel<T, Q>>(
		std::move(access_layout)
	);
}

template <typename T, typename Q>
std::shared_ptr<cpu_kernel> make_copy_kernel(
	array_access_layout,
	std::false_type
)
{
	throw std::invalid_argument(
		"cpu_copy_kernel_builder::build: Input value can not be converted into "
		"the output value"
	);
}

} // anonymous namespace

operation_id 
cpu_copy_kernel_builder::get_operation_id() const noexcept
{
	return operation_id::of<copy_operation>();
}

backend_priority cpu_copy_kernel_builder::get_suitability(
	const operation& operation,
	span<const array_descriptor>,
	hardware::device &device
) const
{
	if (dynamic_cast<const copy_operation*>(&operation) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected operation to be an "
			"instance of copy_operation"
		);
	}

	if (dynamic_cast<const hardware::cpu_device*>(&device) != nullptr)
	{
		return backend_priority::normal;
	}

	return backend_priority::unsupported;
}

std::shared_ptr<kernel> cpu_copy_kernel_builder::build(
	const operation& operation,
	span<const array_descriptor> descriptors,
	hardware::device& device
) const
{
	if (dynamic_cast<const copy_operation*>(&operation) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected operation to be an "
			"instance of copy_operation"
		);
	}

	if (dynamic_cast<const hardware::cpu_device*>(&device) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected device to be an instance "
			"of cpu_device"
		);
	}

	if (descriptors.size() != 2)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected exactly 2 "
			"array descriptors."
		);
	}

	const auto &output_descriptor = descriptors[0];
	const auto &input_descriptor = descriptors[1];

	array_access_layout_builder layout_builder;
	layout_builder.add_operand(output_descriptor.get_layout());
	layout_builder.add_operand(input_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	return visit(
		[&access_layout] 
		(auto output_tag, auto input_tag) -> std::shared_ptr<cpu_kernel>
		{
			using output_value_type = typename decltype(output_tag)::type;
			using input_value_type = typename decltype(input_tag)::type;

			return make_copy_kernel<output_value_type, input_value_type>(
				std::move(access_layout),
				typename std::is_convertible<
					input_value_type, 
					output_value_type
				>::type()
			);
		},
		output_descriptor.get_data_type(),
		input_descriptor.get_data_type()
	);
}

} // namespace multidimensional
} // namespace xmipp4
