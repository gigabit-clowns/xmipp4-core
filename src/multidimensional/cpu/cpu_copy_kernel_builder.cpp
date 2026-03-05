// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>
#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/cpu/cpu_device.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>

#include "cpu_kernel.hpp"

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

	if (descriptors.size() != 2)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected exactly 2 "
			"array descriptors."
		);
	}

	const auto &destination_descriptor = descriptors[0];
	const auto &source_descriptor = descriptors[1];

	array_access_layout_builder layout_builder;
	layout_builder.add_operand(destination_descriptor.get_layout());
	layout_builder.add_operand(source_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	return dispatch_numerical_types(
		[&access_layout] 
		(auto destination_tag, auto source_tag) -> std::shared_ptr<kernel>
		{
			using output_value_type = typename decltype(destination_tag)::type;
			using input_value_type = typename decltype(source_tag)::type;
			
			return make_copy_kernel<output_value_type, input_value_type>(
				std::move(access_layout),
				typename std::is_convertible<
					input_value_type, 
					output_value_type
				>::type()
			);
		},
		destination_descriptor.get_data_type(),
		source_descriptor.get_data_type()
	);
}

} // namespace multidimensional
} // namespace xmipp4
