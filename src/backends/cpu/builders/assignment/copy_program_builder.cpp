// SPDX-License-Identifier: GPL-3.0-only

#include "copy_program_builder.hpp"

#include <xmipp4/ops/assignment/copy_operation.hpp>
#include <xmipp4/core/layout/joint_layout_builder.hpp>
#include <xmipp4/core/ndarray/array_descriptor.hpp>
#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/numerical/numerical_type_dispatch.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>
#include <xmipp4/core/platform/attributes.hpp>
#include <xmipp4/backends/cpu/device.hpp>
#include <xmipp4/backends/cpu/program.hpp>

#include <backends/cpu/hardware/functor_program.hpp>
#include <backends/cpu/loops/elementwise_loop.hpp>
#include <backends/cpu/load_store.hpp>

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
make_copy_program(
	joint_layout layout,
	type_list<T, Q> /*types*/
)
{
	return make_functor_program(
		[layout=std::move(layout)]
		(std::tuple<T*> outputs, std::tuple<const Q*> inputs, std::tuple<>)
		{
			run_elementwise_loop(
				[] (T* destination, const Q* source)
				{
					cast(destination, source);
				},
				layout,
				std::get<ops::copy_operation::OUTPUT_OPERAND_DESTINATION>(outputs),
				std::get<ops::copy_operation::INPUT_OPERAND_SOURCE>(inputs)
			);
		},
		type_list<T>(),
		type_list<Q>()
	);
}

template <typename T, typename Q>
XMIPP4_NORETURN
typename std::enable_if<
	!std::is_constructible<T, Q>::value,
	std::shared_ptr<program>
>::type
make_copy_program(
	joint_layout /*layout*/,
	type_list<T, Q> /*types*/
)
{
	throw std::invalid_argument(
		"copy_program_builder: Can not convert source array's "
		"type into destination array's type."
	);
}

} // anonymous namespace

operation_id
copy_program_builder::get_operation_id() const noexcept
{
	return operation_id::of<ops::copy_operation>();
}

std::shared_ptr<xmipp4::program> copy_program_builder::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	if (!dynamic_cast<const ops::copy_operation*>(&operation))
	{
		throw std::invalid_argument(
			"copy_program_builder::build: Expected operation to "
			"be an instance of ops::copy_operation."
		);
	}

	if (
		output_signatures.size() !=
		ops::copy_operation::OUTPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"copy_program_builder::build: Expected exactly 1 "
			"output signature."
		);
	}

	if (
		input_signatures.size() !=
		ops::copy_operation::INPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"copy_program_builder::build: Expected exactly 1 "
			"input signature."
		);
	}

	const auto &dst_descriptor =
		output_signatures[ops::copy_operation::OUTPUT_OPERAND_DESTINATION]
		.get_descriptor();
	const auto &src_descriptor =
		input_signatures[ops::copy_operation::INPUT_OPERAND_SOURCE]
		.get_descriptor();

	const auto dst_data_type = dst_descriptor.get_data_type();
	const auto src_data_type = src_descriptor.get_data_type();

	joint_layout_builder layout_builder;
	layout_builder.add_operand(dst_descriptor.get_layout());
	layout_builder.add_operand(src_descriptor.get_layout());
	auto layout = layout_builder.build();

	return dispatch_numerical_types(
		[&layout] (auto dst_type_tag, auto src_type_tag)
		{
			using dst_type = typename decltype(dst_type_tag)::type;
			using src_type = typename decltype(src_type_tag)::type;
			return make_copy_program(
				std::move(layout),
				type_list<dst_type, src_type>()
			);
		},
		dst_data_type,
		src_data_type
	);
}

} // namespace cpu
} // namespace xmipp4
