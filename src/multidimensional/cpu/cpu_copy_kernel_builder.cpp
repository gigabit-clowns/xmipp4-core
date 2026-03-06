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


template <typename T, typename Q>
std::shared_ptr<kernel> make_copy_kernel(
	array_access_layout access_layout,
	std::true_type
)
{
	//using kernel_type = cpu_kernel<type_list<T>, type_list<Q>>;
	//return std::make_shared<kernel_type>();
	return nullptr;
}

template <typename T, typename Q>
std::shared_ptr<kernel> make_copy_kernel(
	array_access_layout,
	std::false_type
)
{
	throw std::invalid_argument(
		"cpu_copy_kernel_builder::build: Input value can not be converted into "
		"the output value"
	);
}















template <typename T, typename Q, typename... Strides>
std::shared_ptr<kernel> make_copy_kernel(
	array_access_layout access_layout,
	std::size_t inner_extent,
	const std::tuple<Strides...> inner_strides,
	type_tag<T>,
	type_tag<Q>
)
{
	using destination_type = T;
	using source_type = Q;

	return make_typed_kernel_shared(
		[] (const auto &pointers)
		{
			auto *destination = 
				std::get<copy_operation::OPERAND_DESTINATION>(pointers);
			const auto *source = 
				std::get<copy_operation::OPERAND_SOURCE>(pointers);
			// TODO handle this
		},
		type_list<destination_type>(),
		type_list<source_type>()
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

	array_access_layout_builder layout_builder;
	layout_builder.add_operand(destination_descriptor.get_layout());
	layout_builder.add_operand(source_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	return dispatch_numerical_types(
		[&access_layout] 
		(auto destination_tag, auto source_tag) -> std::shared_ptr<kernel>
		{
			XMIPP4_CONST_CONSTEXPR
			std::integral_constant<std::size_t, copy_operation::OPERAND_COUNT> 
			count;

			return dispatch_inner_loop(
				[&access_layout, destination_tag, source_tag]
				(std::size_t inner_extent, const auto &inner_strides)
				{
					return make_copy_kernel(
						std::move(access_layout),
						inner_extent,
						inner_strides,
						destination_tag,
						source_tag
					);
				},
				access_layout,
				count
			);
		},
		destination_descriptor.get_data_type(),
		source_descriptor.get_data_type()
	);
}

} // namespace multidimensional
} // namespace xmipp4
