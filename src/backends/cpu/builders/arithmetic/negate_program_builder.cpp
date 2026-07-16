// SPDX-License-Identifier: GPL-3.0-only

#include "negate_program_builder.hpp"

#include <xmipp4/ops/arithmetic/negate_operation.hpp>
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
#include <backends/cpu/loops/elementwise_loop.hpp>
#include <backends/cpu/load_store.hpp>

#include <tuple>
#include <complex>
#include <type_traits>

namespace xmipp4
{
namespace cpu
{

namespace
{

// Negate is defined for the signed arithmetic types: signed integers
// (including char8 where char is signed), floating point (including
// float16_t) and complex. Unsigned integers and boolean are rejected.
template <typename T>
struct is_negatable : std::integral_constant<
	bool,
	(std::is_integral<T>::value && std::is_signed<T>::value) ||
	std::is_floating_point<T>::value ||
	std::is_same<T, float16_t>::value
> {};

template <typename T>
struct is_negatable<std::complex<T>> : std::true_type {};

template <typename T>
typename std::enable_if<
	is_negatable<T>::value,
	std::shared_ptr<program>
>::type
make_negate_program(
	joint_layout layout,
	type_list<T> /*types*/
)
{
	return make_functor_program(
		[layout=std::move(layout)]
		(std::tuple<T*> outputs, std::tuple<const T*> inputs, std::tuple<>)
		{
			run_elementwise_loop(
				[] (T* result, const T* x)
				{
					store(result, -load(x));
				},
				layout,
				std::get<ops::negate_operation::OUTPUT_OPERAND_RESULT>(outputs),
				std::get<ops::negate_operation::INPUT_OPERAND_VALUE>(inputs)
			);
		},
		type_list<T>(),
		type_list<T>()
	);
}

template <typename T>
XMIPP4_NORETURN
typename std::enable_if<
	!is_negatable<T>::value,
	std::shared_ptr<program>
>::type
make_negate_program(
	joint_layout /*layout*/,
	type_list<T> /*types*/
)
{
	throw std::invalid_argument(
		"negate_program_builder::build: Expected signed arithmetic type."
	);
}

} // anonymous namespace

operation_id
negate_program_builder::get_operation_id() const noexcept
{
	return operation_id::of<ops::negate_operation>();
}

std::shared_ptr<xmipp4::program> negate_program_builder::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	if (!dynamic_cast<const ops::negate_operation*>(&operation))
	{
		throw std::invalid_argument(
			"negate_program_builder::build: Expected operation to "
			"be an instance of ops::negate_operation."
		);
	}

	if (
		output_signatures.size() !=
		ops::negate_operation::OUTPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"negate_program_builder::build: Expected exactly 1 "
			"output signature."
		);
	}

	if (
		input_signatures.size() !=
		ops::negate_operation::INPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"negate_program_builder::build: Expected exactly 1 "
			"input signature."
		);
	}

	const auto &result_descriptor =
		output_signatures[ops::negate_operation::OUTPUT_OPERAND_RESULT]
		.get_descriptor();
	const auto &x_descriptor =
		input_signatures[ops::negate_operation::INPUT_OPERAND_VALUE]
		.get_descriptor();

	const auto data_type = result_descriptor.get_data_type();
	if (x_descriptor.get_data_type() != data_type)
	{
		throw std::invalid_argument(
			"negate_program_builder::build: Input operand must match output "
			"data type"
		);
	}

	joint_layout_builder layout_builder;
	layout_builder.add_operand(result_descriptor.get_layout());
	layout_builder.add_operand(x_descriptor.get_layout());
	auto layout = layout_builder.build();

	return dispatch_numerical_types(
		[&layout] (auto type_tag)
		{
			using type = typename decltype(type_tag)::type;
			return make_negate_program(
				std::move(layout),
				type_list<type>()
			);
		},
		data_type
	);
}

} // namespace cpu
} // namespace xmipp4
