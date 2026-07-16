// SPDX-License-Identifier: GPL-3.0-only

#include "modulo_program_builder.hpp"

#include <xmipp4/ops/arithmetic/modulo_operation.hpp>
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
#include <backends/cpu/type_maps.hpp>

#include <tuple>
#include <type_traits>

namespace xmipp4
{
namespace cpu
{

namespace
{

/*
 * C's "%"/fmod() give a result with the same sign as the dividend.
 * Python's modulo gives a result with the same sign as the divisor
 * instead. The overloads below adjust the C result accordingly, split
 * by category, as unsigned types can never require an adjustment.
 */

template <typename T>
inline T to_divisor_sign(T remainder, T divisor)
{
	return (remainder != T(0) && (remainder < T(0)) != (divisor < T(0))) ?
		remainder + divisor : remainder;
}

template <typename T>
inline
typename std::enable_if<
	std::is_integral<T>::value && std::is_unsigned<T>::value,
	T
>::type
floor_mod(T x, T y)
{
	return x % y;
}

template <typename T>
inline
typename std::enable_if<
	std::is_integral<T>::value && std::is_signed<T>::value,
	T
>::type
floor_mod(T x, T y)
{
	return to_divisor_sign(static_cast<T>(x % y), y);
}

template <typename T>
inline
typename std::enable_if<!std::is_integral<T>::value, T>::type
floor_mod(T x, T y)
{
	using std::fmod;
	return to_divisor_sign(fmod(x, y), y);
}

template <typename T>
std::shared_ptr<program> make_modulo_program(
	joint_layout layout,
	type_list<T> /*types*/
)
{
	return make_functor_program(
		[layout=std::move(layout)]
		(std::tuple<T*> outputs, std::tuple<const T*, const T*> inputs, std::tuple<>)
		{
			run_elementwise_loop(
				[] (T* result, const T* x, const T* y)
				{
					*result = floor_mod(*x, *y);
				},
				layout,
				std::get<ops::modulo_operation::OUTPUT_OPERAND_RESULT>(outputs),
				std::get<ops::modulo_operation::INPUT_OPERAND_DIVIDEND>(inputs),
				std::get<ops::modulo_operation::INPUT_OPERAND_DIVISOR>(inputs)
			);
		},
		type_list<T>(),
		type_list<T, T>()
	);
}

XMIPP4_NORETURN
std::shared_ptr<program> make_modulo_program(
	joint_layout /*layout*/,
	type_list<void> /*types*/
)
{
	throw std::invalid_argument(
		"modulo_program_builder::build: Expected arithmetic type."
	);
}

} // anonymous namespace

operation_id
modulo_program_builder::get_operation_id() const noexcept
{
	return operation_id::of<ops::modulo_operation>();
}

std::shared_ptr<xmipp4::program> modulo_program_builder::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	if (!dynamic_cast<const ops::modulo_operation*>(&operation))
	{
		throw std::invalid_argument(
			"modulo_program_builder::build: Expected operation to "
			"be an instance of ops::modulo_operation."
		);
	}

	if (
		output_signatures.size() !=
		ops::modulo_operation::OUTPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"modulo_program_builder::build: Expected exactly 1 "
			"output signature."
		);
	}

	if (
		input_signatures.size() !=
		ops::modulo_operation::INPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"modulo_program_builder::build: Expected exactly 2 "
			"input signatures."
		);
	}

	const auto &result_descriptor =
		output_signatures[ops::modulo_operation::OUTPUT_OPERAND_RESULT]
		.get_descriptor();
	const auto &x_descriptor =
		input_signatures[ops::modulo_operation::INPUT_OPERAND_DIVIDEND]
		.get_descriptor();
	const auto &y_descriptor =
		input_signatures[ops::modulo_operation::INPUT_OPERAND_DIVISOR]
		.get_descriptor();

	const auto data_type = result_descriptor.get_data_type();
	if (x_descriptor.get_data_type() != data_type)
	{
		throw std::invalid_argument(
			"modulo_program_builder::build: First operand must match output "
			"data type"
		);
	}

	if (y_descriptor.get_data_type() != data_type)
	{
		throw std::invalid_argument(
			"modulo_program_builder::build: Second operand must match output "
			"data type"
		);
	}

	joint_layout_builder layout_builder;
	layout_builder.add_operand(result_descriptor.get_layout());
	layout_builder.add_operand(x_descriptor.get_layout());
	layout_builder.add_operand(y_descriptor.get_layout());
	auto layout = layout_builder.build();

	return dispatch_numerical_types(
		[&layout] (auto type_tag)
		{
			using type = typename decltype(type_tag)::type;
			return make_modulo_program(
				std::move(layout),
				type_list<type>()
			);
		},
		type_map_cat<native_integer_type_map, compute_floating_type_map>::type(),
		data_type
	);
}

} // namespace cpu
} // namespace xmipp4
