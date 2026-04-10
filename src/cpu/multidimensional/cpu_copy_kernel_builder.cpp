// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>
#include <xmipp4/core/multidimensional/multi_array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/cpu/hardware/cpu_device.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>

#include "cpu_kernel.hpp"
#include "cpu_inner_loop_dispatch.hpp"
#include "cpu_elementwise_outer_loop.hpp"
#include "cpu_array_signature_check.hpp"
#include "kernels/generic/copy.hpp"
#include "kernels/highway/fill_constant_kernel.hpp"
#include "kernels/highway/helpers/convert_data_type.hpp"

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

template <typename T, typename Q>
std::shared_ptr<kernel> make_copy_kernel(
	multi_array_access_layout access_layout,
	std::tuple<contiguous_stride_tag, contiguous_stride_tag> /*inner_strides*/,
	type_tag<T> /*destination_type_tag*/,
	type_tag<Q> /*source_type_tag*/
)
{
	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[] (T *destination, const Q *source, std::size_t count)
			{
				cast_contiguous(destination, source, count);
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<Q>()
	);
}

template <typename T>
std::shared_ptr<kernel> make_copy_kernel(
	multi_array_access_layout access_layout,
	std::tuple<contiguous_stride_tag, contiguous_stride_tag> /*inner_strides*/,
	type_tag<T> /*destination_type_tag*/,
	type_tag<T> /*source_type_tag*/
)
{
	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[] (T *destination, const T *source, std::size_t count)
			{
				std::copy_n(source, count, destination);
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<T>()
	);
}

template <typename T, typename Q>
std::shared_ptr<kernel> make_copy_kernel(
	multi_array_access_layout access_layout,
	std::tuple<contiguous_stride_tag, broadcasting_stride_tag> /*inner_strides*/,
	type_tag<T> /*destination_type_tag*/,
	type_tag<Q> /*source_type_tag*/
)
{
	fill_constant_kernel<typename to_hwy_data_type<T>::type> fill;
	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[fill] (T *destination, const Q *source, std::size_t count)
			{
				const auto fill_value = static_cast<T>(*source);
				fill(to_hwy(destination), count, *to_hwy(&fill_value));
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<Q>()
	);
}

template <typename Q>
std::shared_ptr<kernel> make_copy_kernel(
	multi_array_access_layout access_layout,
	std::tuple<contiguous_stride_tag, broadcasting_stride_tag> /*inner_strides*/,
	type_tag<bool> /*destination_type_tag*/,
	type_tag<Q> /*source_type_tag*/
)
{
	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[] (bool *destination, const Q *source, std::size_t count)
			{
				const auto fill_value = static_cast<bool>(*source);
				std::fill_n(destination, count, fill_value);
			},
			std::move(access_layout)
		),
		type_list<bool>(),
		type_list<Q>()
	);
}

template <typename Q>
std::shared_ptr<kernel> make_copy_kernel(
	multi_array_access_layout access_layout,
	std::tuple<contiguous_stride_tag, broadcasting_stride_tag> /*inner_strides*/,
	type_tag<char> /*destination_type_tag*/,
	type_tag<Q> /*source_type_tag*/
)
{
	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[] (char *destination, const Q *source, std::size_t count)
			{
				const auto fill_value = static_cast<char>(*source);
				std::fill_n(destination, count, fill_value);
			},
			std::move(access_layout)
		),
		type_list<char>(),
		type_list<Q>()
	);
}

template <typename T, typename Q, typename DstStrideT, typename SrcStrideT>
std::shared_ptr<kernel> make_copy_kernel(
	multi_array_access_layout access_layout,
	const std::tuple<DstStrideT, SrcStrideT>& inner_strides,
	type_tag<T> /*destination_type_tag*/,
	type_tag<Q> /*source_type_tag*/
)
{
	const auto destination_inner_stride = 
		std::get<copy_operation::OPERAND_DESTINATION>(inner_strides);
	const auto source_inner_stride = 
		std::get<copy_operation::OPERAND_SOURCE>(inner_strides);

	return make_cpu_kernel_shared(
		make_cpu_outer_loop(
			[destination_inner_stride, source_inner_stride] 
			(T *destination, const Q* source, std::size_t count)
			{
				cast_strided(
					destination,
					source,
					count,
					destination_inner_stride,
					source_inner_stride
				);
			},
			std::move(access_layout)
		),
		type_list<T>(),
		type_list<Q>()
	);
}

template <typename F, typename T, typename Q>
std::shared_ptr<kernel> cast_handler(
	F&& callable,
	type_tag<T> destination_type_tag,
	type_tag<Q> source_type_tag,
	std::true_type
)
{
	return std::forward<F>(callable)(destination_type_tag, source_type_tag);
}

template <typename F, typename T, typename Q>
std::shared_ptr<kernel> cast_handler(
	F&&,
	type_tag<T>,
	type_tag<Q>,
	std::false_type
)
{
	throw std::invalid_argument(
		"Provided source array's data type is not castable to destination "
		"array's data type"
	);
}

template <typename F, typename T, typename Q>
std::shared_ptr<kernel> cast_handler(
	F&& callable,
	type_tag<T> destination_type_tag,
	type_tag<Q> source_type_tag
)
{
	return cast_handler(
		std::forward<F>(callable),
		destination_type_tag,
		source_type_tag,
		typename std::is_convertible<Q, T>::type()
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
	span<const array_signature> output_signatures,
	span<const array_signature> input_signatures
) const
{
	if (cpu_check_array_signatures(output_signatures, input_signatures))
	{
		return backend_priority::fallback;
	}

	return backend_priority::unsupported;
}

std::shared_ptr<kernel> cpu_copy_kernel_builder::build(
	const operation& operation,
	span<const array_signature> output_signatures,
	span<const array_signature> input_signatures
) const
{
	if (dynamic_cast<const copy_operation*>(&operation) == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected operation to be an "
			"instance of copy_operation"
		);
	}

	if (output_signatures.size() != copy_operation::OUTPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected exactly 1 "
			"output array signatures."
		);
	}
	if (output_signatures.size() != copy_operation::INPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected exactly 1 "
			"input array signatures."
		);
	}

	const auto &destination_descriptor = 
		output_signatures[copy_operation::OUTPUT_OPERAND_DESTINATION];
	const auto &source_descriptor = 
		input_signatures[copy_operation::INPUT_OPERAND_SOURCE];

	multi_array_access_layout_builder layout_builder;
	layout_builder.add_operand(destination_descriptor.get_layout());
	layout_builder.add_operand(source_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	return dispatch_numerical_types(
		[&access_layout] 
		(auto destination_type_tag, auto source_type_tag)
		{
			return cast_handler(
				[&access_layout] 
				(auto destination_type_tag, auto source_type_tag)
				{
					return dispatch_inner_loop(
						[&access_layout, destination_type_tag, source_type_tag]
						(const auto &inner_strides)
						{
							return make_copy_kernel(
								std::move(access_layout),
								inner_strides,
								destination_type_tag,
								source_type_tag
							);
						},
						access_layout,
						copy_operand_count_tag()
					);
				},
				destination_type_tag,
				source_type_tag
			);
		},
		native_type_map(),
		destination_descriptor.get_data_type(),
		source_descriptor.get_data_type()
	);
}

} // namespace multidimensional
} // namespace xmipp4
