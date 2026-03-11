// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>
#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/cpu/cpu_device.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>

#include "cpu_kernel.hpp"
#include "cpu_inner_loop_dispatch.hpp"
#include "cpu_outer_loop.hpp"

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

template <typename T>
struct cpu_copy
{
	void operator()(
		T *destination, 
		const T* source, 
		std::size_t count,
		std::ptrdiff_t destination_stride,
		std::ptrdiff_t source_stride
	) const
	{
		std::ptrdiff_t destination_index = 0;
		std::ptrdiff_t source_index = 0;
		for (std::size_t i = 0; i < count; ++i)
		{
			destination[destination_index] = source[source_index];

			destination_index += destination_stride;
			source_index += source_stride;
		}
	}

	void operator()(
		T *destination, 
		const T* source, 
		std::size_t count,
		std::integral_constant<std::ptrdiff_t, 1>,
		std::integral_constant<std::ptrdiff_t, 1>
	) const
	{
		std::copy_n(source, count, destination);
	}

	void operator()(
		T *destination, 
		const T* source, 
		std::size_t count,
		std::integral_constant<std::ptrdiff_t, 1>,
		std::integral_constant<std::ptrdiff_t, 0>
	) const
	{
		std::fill(destination, destination + count, *source);
	}
};

template <typename T, typename... Strides>
std::shared_ptr<kernel> make_copy_kernel(
	array_access_layout access_layout,
	std::size_t inner_extent,
	const std::tuple<Strides...> inner_strides,
	type_tag<T>
)
{
	return make_typed_kernel_shared(
		make_cpu_outer_loop(
			cpu_copy<T>(),
			std::move(access_layout),
			inner_extent,
			inner_strides
		),
		type_list<T>(),
		type_list<T>()
	);
}

} // anonymous namespace

operation_id 
cpu_copy_kernel_builder::get_operation_id() const noexcept
{
	return operation_id::of<copy_operation>();
}

backend_priority cpu_copy_kernel_builder::get_suitability(
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

	if (descriptors.size() != copy_operation::OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected exactly 2 "
			"array descriptors."
		);
	}

	const auto &destination_descriptor = 
		descriptors[copy_operation::OPERAND_DESTINATION];
	const auto &source_descriptor = 
		descriptors[copy_operation::OPERAND_SOURCE];

	const auto data_type = destination_descriptor.get_data_type();
	if (source_descriptor.get_data_type() != data_type)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected source and destination "
			"operands to have same data type."
		);
	}

	array_access_layout_builder layout_builder;
	layout_builder.add_operand(destination_descriptor.get_layout());
	layout_builder.add_operand(source_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	return dispatch_numerical_types(
		[&access_layout] 
		(auto tag) -> std::shared_ptr<kernel>
		{
			XMIPP4_CONST_CONSTEXPR
			std::integral_constant<std::size_t, copy_operation::OPERAND_COUNT> 
			operand_count;

			return dispatch_inner_loop(
				[&access_layout, tag]
				(std::size_t inner_extent, const auto &inner_strides)
				{
					return make_copy_kernel(
						std::move(access_layout),
						inner_extent,
						inner_strides,
						tag
					);
				},
				access_layout,
				operand_count
			);
		},
		data_type
	);
}

} // namespace multidimensional
} // namespace xmipp4
