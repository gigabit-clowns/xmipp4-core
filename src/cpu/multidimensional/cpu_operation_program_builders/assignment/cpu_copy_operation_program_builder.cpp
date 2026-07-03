// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_operation_program_builder.hpp"

#include <xmipp4/core/multidimensional/operations/assignment/copy_operation.hpp>
#include <xmipp4/core/multidimensional/multi_array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>
#include <xmipp4/core/scalar_value.hpp>
#include <xmipp4/core/numerical_cast.hpp>
#include <xmipp4/cpu/hardware/cpu_device.hpp>
#include <xmipp4/cpu/hardware/cpu_program.hpp>

#include <cpu/hardware/functor_cpu_program.hpp>
#include <cpu/multidimensional/cpu_elementwise_outer_loop.hpp>
#include <cpu/multidimensional/cpu_inner_loop_stride_dispatch.hpp>
#include <cpu/multidimensional/helpers/strided_pointer_iterator.hpp>

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

template <
	typename T, 
	typename Q, 
	typename DstStride, 
	typename SrcStride
>
void copy(
	T* dst,
	const Q* src,
	std::size_t count,
	DstStride dst_stride, 
	SrcStride src_stride
)
{
	auto dst_ite = make_strided_pointer_iterator(dst, dst_stride);
	auto src_ite = make_strided_pointer_iterator(src, src_stride);
	for (std::size_t i = 0; i < count; ++i)
	{
		*dst_ite = numerical_cast<T>(*src_ite);
		++dst_ite;
		++src_ite;
	}
}

template<typename T>
void copy(
	T* dst,
	const T* src,
	std::size_t count,
	contiguous_stride_tag /*dst_stride*/, 
	contiguous_stride_tag /*src_stride*/
)
{
	std::copy_n(
		src,
		count,
		dst
	);
}

template<typename T, typename Q>
void copy(
	T* dst,
	const Q* src,
	std::size_t count,
	contiguous_stride_tag /*dst_stride*/, 
	broadcasting_stride_tag /*src_stride*/
)
{
	std::fill_n(
		dst,
		count,
		numerical_cast<T>(*src)
	);
}

template <
	typename T, 
	typename Q, 
	typename DstStride, 
	typename SrcStride
>
typename std::enable_if<
	std::is_constructible<T, Q>::value, 
	std::shared_ptr<hardware::cpu_program>
>::type
make_copy_program(
	multi_array_access_layout access_layout,
	std::tuple<DstStride, SrcStride> inner_strides,
	type_tag<T> /*dst_type*/,
	type_tag<Q> /*src_type*/
)
{
	const auto dst_inner_stride = std::get<0>(inner_strides);
	const auto src_inner_stride = std::get<1>(inner_strides);
	return hardware::make_functor_cpu_program(
		[dst_inner_stride, src_inner_stride, access_layout=std::move(access_layout)]
		(std::tuple<T*> outputs, std::tuple<const Q*> inputs, std::tuple<>)
		{
			run_elementwise_outer_loop(
				[dst_inner_stride, src_inner_stride] 
				(T *dst, const Q* src, const std::size_t count)
				{
					copy(dst, src, count, dst_inner_stride, src_inner_stride);
				},
				access_layout,
				std::get<copy_operation::OUTPUT_OPERAND_DESTINATION>(outputs),
				std::get<copy_operation::INPUT_OPERAND_SOURCE>(inputs)
			);
		},
		type_list<T>(),
		type_list<Q>()
	);
}

template <
	typename T, 
	typename Q, 
	typename DstStride, 
	typename SrcStride
>
typename std::enable_if<
	!std::is_constructible<T, Q>::value, 
	std::shared_ptr<hardware::cpu_program>
>::type
make_copy_program(
	multi_array_access_layout /*access_layout*/,
	std::tuple<DstStride, SrcStride> /*inner_strides*/,
	type_tag<T> /*dst_type*/,
	type_tag<Q> /*src_type*/
)
{
	throw std::invalid_argument(
		"cpu_copy_operation_program_builder: Can not convert source array's "
		"type into destination array's type."
	);
}

} // anonymous namespace

operation_id 
cpu_copy_operation_program_builder::get_operation_id() const noexcept
{
	return operation_id::of<copy_operation>();
}

std::shared_ptr<hardware::program> cpu_copy_operation_program_builder::build(
	const operation &operation,
	span<const array_signature> output_signatures,
	span<const array_signature> input_signatures,
	hardware::command_queue& /*queue*/,
	operation_program_cache* /*cache*/
) const
{
	if (!dynamic_cast<const copy_operation*>(&operation))
	{
		throw std::invalid_argument(
			"cpu_copy_operation_program_builder::build: Expected operation to "
			"be an instance of copy_operation."
		);
	}

	if (output_signatures.size() != copy_operation::OUTPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_copy_operation_program_builder::build: Expected exactly 1 "
			"output signature."
		);
	}

	if (input_signatures.size() != copy_operation::INPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_copy_operation_program_builder::build: Expected exactly 1 "
			"input signature."
		);
	}

	const auto &dst_descriptor = 
		output_signatures[copy_operation::OUTPUT_OPERAND_DESTINATION]
		.get_descriptor();
	const auto &src_descriptor = 
		output_signatures[copy_operation::INPUT_OPERAND_SOURCE]
		.get_descriptor();

	const auto dst_data_type = dst_descriptor.get_data_type();
	const auto src_data_type = src_descriptor.get_data_type();

	multi_array_access_layout_builder layout_builder;
	layout_builder.add_operand(dst_descriptor.get_layout());
	layout_builder.add_operand(src_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	return dispatch_numerical_types(
		[&access_layout] (auto dst_type_tag, auto src_type_tag)
		{
			return dispatch_inner_loop_strides(
				[&access_layout, dst_type_tag, src_type_tag] 
				(auto inner_strides)
				{
					return make_copy_program(
						std::move(access_layout),
						inner_strides,
						dst_type_tag,
						src_type_tag
					);
				},
				access_layout,
				std::integral_constant<std::size_t, 2>()
			);
		},
		native_type_map(),
		dst_data_type, 
		src_data_type
	);
}

} // namespace multidimensional
} // namespace xmipp4
