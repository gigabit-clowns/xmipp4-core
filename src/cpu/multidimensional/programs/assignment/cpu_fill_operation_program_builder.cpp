// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_fill_operation_program_builder.hpp"

#include <xmipp4/core/multidimensional/operations/assignment/fill_operation.hpp>
#include <xmipp4/core/multidimensional/multi_array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>
#include <xmipp4/core/scalar_value.hpp>
#include <xmipp4/cpu/hardware/cpu_device.hpp>
#include <xmipp4/cpu/hardware/cpu_program.hpp>

#include <cpu/hardware/functor_cpu_program.hpp>
#include <cpu/hardware/cpu_command_queue.hpp>
#include <cpu/multidimensional/cpu_elementwise_outer_loop.hpp>
#include <cpu/multidimensional/cpu_inner_loop_stride_dispatch.hpp>
#include <cpu/multidimensional/helpers/strided_pointer_iterator.hpp>
#include <cpu/multidimensional/helpers/cpu_array_signature_check.hpp>

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

template<
	typename T, 
	typename Q 
>
typename std::enable_if<std::is_constructible<T, Q>::value, T>::type
cast_or_throw(const Q& value)
{
	return T(value);
}

template<
	typename T, 
	typename Q
>
typename std::enable_if<!std::is_constructible<T, Q>::value, T>::type
cast_or_throw(const Q& /*value*/)
{
	throw std::invalid_argument(
		"Can not convert fill_value to destination array's type"
	);
}

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
std::shared_ptr<hardware::cpu_program> make_fill_program(
	multi_array_access_layout access_layout,
	std::tuple<Stride> inner_strides,
	type_tag<T> /*result_type*/,
	const Q &fill_value
)
{
	const auto value = cast_or_throw<T>(fill_value);
	const auto result_inner_stride = std::get<0>(inner_strides);
	return hardware::make_functor_cpu_program(
		[result_inner_stride, value, access_layout=std::move(access_layout)]
		(std::tuple<T*> outputs, std::tuple<>, std::tuple<>)
		{
			run_elementwise_outer_loop(
				[result_inner_stride, value] (T *result, std::size_t count)
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

} // anonymous namespace

operation_id 
cpu_fill_operation_program_builder::get_operation_id() const noexcept
{
	return operation_id::of<fill_operation>();
}

backend_priority cpu_fill_operation_program_builder::get_suitability(
	const operation& /*operation*/,
	span<const array_signature> output_signatures,
	span<const array_signature> input_signatures,
	hardware::command_queue &queue
) const
{
	if (!cpu_check_array_signatures(output_signatures))
	{
		return backend_priority::unsupported;
	}

	if (!cpu_check_array_signatures(input_signatures))
	{
		return backend_priority::unsupported;
	}

	if (!hardware::cpu_command_queue::try_cast(queue))
	{
		return backend_priority::unsupported;
	}

	return backend_priority::normal;
}

std::shared_ptr<hardware::program> cpu_fill_operation_program_builder::build(
	const operation &operation,
	span<const array_signature> output_signatures,
	span<const array_signature> input_signatures,
	hardware::command_queue& /*queue*/,
	operation_program_cache* /*cache*/
) const
{
	const auto *fill_op = 
		dynamic_cast<const fill_operation*>(&operation);
	if (fill_op == nullptr)
	{
		throw std::invalid_argument(
			"cpu_fill_operation_program_builder::build: Expected operation to "
			"be an instance of fill_operation."
		);
	}

	if (output_signatures.size() != fill_operation::OUTPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_fill_operation_program_builder::build: Expected exactly 1 "
			"output signature."
		);
	}

	if (input_signatures.size() != fill_operation::INPUT_OPERAND_COUNT)
	{
		throw std::invalid_argument(
			"cpu_fill_operation_program_builder::build: Expected no input "
			"signature."
		);
	}

	const auto &destination_descriptor = 
		output_signatures[fill_operation::OUTPUT_OPERAND_DESTINATION]
		.get_descriptor();

	const auto data_type = destination_descriptor.get_data_type();
	const auto &fill_value = fill_op->get_fill_value();

	multi_array_access_layout_builder layout_builder;
	layout_builder.add_operand(destination_descriptor.get_layout());
	auto access_layout = layout_builder.build();

	return xmipp4::visit(
		[&access_layout, data_type] (auto fill_value)
		{
			return dispatch_numerical_types(
				[&access_layout, &fill_value] 
				(auto destination_type_tag)
				{
					return dispatch_inner_loop_strides(
						[&access_layout, &fill_value, destination_type_tag] 
						(auto inner_strides)
						{
							return make_fill_program(
								std::move(access_layout),
								inner_strides,
								destination_type_tag,
								fill_value
							);
						},
						access_layout,
						std::integral_constant<std::size_t, 1>()
					);
				},
				native_type_map(),
				data_type
			);
		},
		fill_value
	);
}

} // namespace multidimensional
} // namespace xmipp4
