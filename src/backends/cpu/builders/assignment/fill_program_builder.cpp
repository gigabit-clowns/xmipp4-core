// SPDX-License-Identifier: GPL-3.0-only

#include "fill_program_builder.hpp"

#include <xmipp4/ops/assignment/fill_operation.hpp>
#include <xmipp4/core/layout/joint_layout_builder.hpp>
#include <xmipp4/core/ndarray/array_descriptor.hpp>
#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/numerical/numerical_type_dispatch.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>
#include <xmipp4/core/numerical/numerical_cast.hpp>
#include <xmipp4/core/platform/attributes.hpp>
#include <xmipp4/backends/cpu/device.hpp>
#include <xmipp4/backends/cpu/program.hpp>

#include <backends/cpu/hardware/functor_program.hpp>
#include <backends/cpu/hardware/command_queue.hpp>
#include <backends/cpu/loops/elementwise_loop.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace xmipp4
{
namespace cpu
{

namespace
{

template <typename T, typename Q>
typename std::enable_if<
	std::is_constructible<T, Q>::value,
	std::shared_ptr<program>
>::type
make_fill_program(
	joint_layout layout,
	type_list<T> /*result_type*/,
	const Q &fill_value
)
{
	const auto value = numerical_cast<T>(fill_value);
	return make_functor_program(
		[value, layout=std::move(layout)]
		(std::tuple<T*> outputs, std::tuple<>, std::tuple<>)
		{
			run_elementwise_loop(
				[value] (T* destination) { *destination = value; },
				layout,
				std::get<ops::fill_operation::OUTPUT_OPERAND_DESTINATION>(outputs)
			);
		},
		type_list<T>(),
		type_list<>()
	);
}

template <typename T, typename Q>
XMIPP4_NORETURN
typename std::enable_if<
	!std::is_constructible<T, Q>::value,
	std::shared_ptr<program>
>::type
make_fill_program(
	joint_layout /*layout*/,
	type_list<T> /*result_type*/,
	const Q& /*fill_value*/
)
{
	throw std::invalid_argument(
		"fill_program_builder: Can not fill value's type into "
		"destination array's type."
	);
}

} // anonymous namespace

operation_id
fill_program_builder::get_operation_id() const noexcept
{
	return operation_id::of<ops::fill_operation>();
}

std::shared_ptr<xmipp4::program> fill_program_builder::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	const auto *fill_op =
		dynamic_cast<const ops::fill_operation*>(&operation);
	if (fill_op == nullptr)
	{
		throw std::invalid_argument(
			"fill_program_builder::build: Expected operation to "
			"be an instance of ops::fill_operation."
		);
	}

	if (
		output_signatures.size() !=
		ops::fill_operation::OUTPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"fill_program_builder::build: Expected exactly 1 "
			"output signature."
		);
	}

	if (
		input_signatures.size() !=
		ops::fill_operation::INPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"fill_program_builder::build: Expected no input "
			"signature."
		);
	}

	const auto &destination_descriptor =
		output_signatures[ops::fill_operation::OUTPUT_OPERAND_DESTINATION]
		.get_descriptor();

	const auto data_type = destination_descriptor.get_data_type();
	const auto &fill_value = fill_op->get_fill_value();

	joint_layout_builder layout_builder;
	layout_builder.add_operand(destination_descriptor.get_layout());
	auto layout = layout_builder.build();

	return dispatch_numerical_types(
		[&layout, &fill_value] (auto result_type_tag)
		{
			using result_type = typename decltype(result_type_tag)::type;
			return xmipp4::visit(
				[&layout] (auto value)
				{
					return make_fill_program(
						std::move(layout),
						type_list<result_type>(),
						value
					);
				},
				fill_value
			);
		},
		native_type_map(),
		data_type
	);
}

} // namespace cpu
} // namespace xmipp4
