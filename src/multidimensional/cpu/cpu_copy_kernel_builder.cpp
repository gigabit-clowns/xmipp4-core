// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>
#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/cpu/cpu_device.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>

#include "cpu_kernel.hpp"
#include "cpu_inner_loop_dispatch.hpp"

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

struct cpu_copy
{
	template <typename T, typename Q, typename DstStrideT, typename SrcStrideT>
	void operator()(
		T* dst, 
		const Q* src,
		std::size_t count, 
		DstStrideT dst_stride,
		SrcStrideT src_stride
	) const noexcept
	{
		std::ptrdiff_t dst_index = 0;
		std::ptrdiff_t src_index = 0;
		for (std::size_t i = 0; i < count; ++i)
		{
			dst[dst_index] = static_cast<T>(src[src_index]);
			dst_index += dst_stride;
			src_index += src_stride;
		}
	}

	template <typename T>
	void operator()(
		T* output, 
		const T* input,
		std::size_t count, 
		std::integral_constant<std::ptrdiff_t, 1>,
		std::integral_constant<std::ptrdiff_t, 1>
	) const noexcept
	{
		std::copy_n(input, count, output);
	}

	template <typename T, typename Q>
	void operator()(
		T* output, 
		const Q* input,
		std::size_t count, 
		std::integral_constant<std::ptrdiff_t, 1>,
		std::integral_constant<std::ptrdiff_t, 1>
	) const noexcept
	{
		std::transform(
			input,
			input + count,
			output,
			[] (const auto &item) { return static_cast<T>(item); }
		);
	}

	template <typename T, typename Q>
	void operator()(
		T* output, 
		const Q* input,
		std::size_t count, 
		std::integral_constant<std::ptrdiff_t, 1>,
		std::integral_constant<std::ptrdiff_t, 0>
	) const noexcept
	{
		const auto input_value = static_cast<T>(*input);
		std::fill_n(output, count, input_value);
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
		[access_layout=std::move(access_layout), inner_extent, &inner_strides] 
		(const auto &pointers)
		{
			auto *destination = 
				std::get<copy_operation::OPERAND_DESTINATION>(pointers);
			const auto *source = 
				std::get<copy_operation::OPERAND_SOURCE>(pointers);
			// TODO handle this
		},
		type_list<T>(),
		type_list<T>()
	);
	return nullptr;
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
