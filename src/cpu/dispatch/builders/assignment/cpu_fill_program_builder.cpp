// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_fill_program_builder.hpp"

#include <xmipp4/ndarray/operations/assignment/fill_operation.hpp>
#include <xmipp4/core/layout/access_layout_builder.hpp>
#include <xmipp4/core/ndarray/array_descriptor.hpp>
#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/numerical/numerical_type_dispatch.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>
#include <xmipp4/core/numerical/numerical_cast.hpp>
#include <xmipp4/cpu/hardware/cpu_device.hpp>
#include <xmipp4/cpu/hardware/cpu_program.hpp>

#include <cpu/hardware/functor_cpu_program.hpp>
#include <cpu/hardware/cpu_command_queue.hpp>
#include <cpu/dispatch/helpers/elementwise_outer_loop.hpp>
#include <cpu/dispatch/helpers/inner_loop_stride_dispatch.hpp>
#include <cpu/dispatch/helpers/type_and_inner_stride_dispatch.hpp>
#include <cpu/dispatch/helpers/strided_pointer_iterator.hpp>

#include <algorithm>

namespace xmipp4
{

namespace
{

template<typename T, typename Stride>
void fill(
	T* destination,
	std::size_t count,
	Stride destination_stride, 
	const T &value
)
{
	std::fill_n(
		make_strided_pointer_iterator(destination, destination_stride),
		count,
		value
	);
}

template<typename T>
void fill(
	T* destination,
	std::size_t count,
	contiguous_stride_tag /*destination_stride*/, 
	const T &value
)
{
	std::fill_n(
		destination,
		count,
		value
	);
}

template <typename T, typename Q, typename Stride>
typename std::enable_if<
	std::is_constructible<T, Q>::value, 
	std::shared_ptr<cpu_program>
>::type
make_fill_program(
	access_layout access_layout,
	std::tuple<Stride> inner_strides,
	type_list<T> /*result_type*/,
	const Q &fill_value
)
{
	const auto value = numerical_cast<T>(fill_value);
	const auto result_inner_stride = std::get<0>(inner_strides);
	return make_functor_cpu_program(
		[result_inner_stride, value, access_layout=std::move(access_layout)]
		(std::tuple<T*> outputs, std::tuple<>, std::tuple<>)
		{
			run_elementwise_outer_loop(
				[result_inner_stride, &value] (T *result, std::size_t count)
				{
					fill(result, count, result_inner_stride, value);
				},
				access_layout,
				std::get<fill_operation::OUTPUT_OPERAND_DESTINATION>(outputs)
			);
		},
		type_list<T>(),
		type_list<>()
	);
}

template <typename T, typename Q, typename Stride>
typename std::enable_if<
	!std::is_constructible<T, Q>::value, 
	std::shared_ptr<cpu_program>
>::type
make_fill_program(
	access_layout /*access_layout*/,
	std::tuple<Stride> /*inner_strides*/,
	type_list<T> /*result_type*/,
	const Q& /*fill_value*/
)
{
	throw std::invalid_argument(
		"cpu_fill_program_builder: Can not fill value's type into "
		"destination array's type."
	);
}

} // anonymous namespace

operation_id
cpu_fill_program_builder::get_operation_id() const noexcept
{
	return operation_id::of<fill_operation>();
}

std::shared_ptr<program> cpu_fill_program_builder::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	const auto *fill_op =
		dynamic_cast<const fill_operation*>(&operation);
	if (fill_op == nullptr)
	{
		throw std::invalid_argument(
			"cpu_fill_program_builder::build: Expected operation to "
			"be an instance of fill_operation."
		);
	}

	if (
		output_signatures.size() !=
		fill_operation::OUTPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"cpu_fill_program_builder::build: Expected exactly 1 "
			"output signature."
		);
	}

	if (
		input_signatures.size() !=
		fill_operation::INPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"cpu_fill_program_builder::build: Expected no input "
			"signature."
		);
	}

	const auto &destination_descriptor =
		output_signatures[fill_operation::OUTPUT_OPERAND_DESTINATION]
		.get_descriptor();

	const auto data_type = destination_descriptor.get_data_type();
	const auto &fill_value = fill_op->get_fill_value();

	access_layout_builder layout_builder;
	layout_builder.add_operand(destination_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	return dispatch_types_and_inner_strides<1>(
		[&fill_value]
		(xmipp4::access_layout layout, auto types, auto inner_strides)
		{
			return xmipp4::visit(
				[&layout, types, &inner_strides] (auto fill_value)
				{
					return make_fill_program(
						std::move(layout),
						inner_strides,
						types,
						fill_value
					);

				},
				fill_value
			);
		},
		std::move(access_layout),
		data_type
	);
}

} // namespace xmipp4
