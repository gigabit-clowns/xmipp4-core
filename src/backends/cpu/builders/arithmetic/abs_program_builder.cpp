// SPDX-License-Identifier: GPL-3.0-only

#include "abs_program_builder.hpp"

#include <xmipp4/ops/arithmetic/abs_operation.hpp>
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

/*
 * std::abs() has no overload for unsigned types and is ambiguous when
 * called with one (it would need to promote to a signed type), even
 * though the value is trivially its own absolute value.
 */

template <typename T>
inline typename std::enable_if<
	std::is_integral<T>::value && std::is_unsigned<T>::value,
	T
>::type
compute_abs(T x)
{
	return x;
}

template <typename T>
inline typename std::enable_if<
	!std::is_integral<T>::value || std::is_signed<T>::value,
	T
>::type
compute_abs(T x)
{
	using std::abs;
	return static_cast<T>(abs(x));
}



template <typename T>
std::shared_ptr<program> make_abs_program(
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
					store(result, compute_abs(load(x)));
				},
				layout,
				std::get<ops::abs_operation::OUTPUT_OPERAND_RESULT>(outputs),
				std::get<ops::abs_operation::INPUT_OPERAND_VALUE>(inputs)
			);
		},
		type_list<T>(),
		type_list<T>()
	);
}

template <typename T>
std::shared_ptr<program> make_abs_program(
	joint_layout layout,
	type_list<std::complex<T>> /*types*/
)
{
	return make_functor_program(
		[layout=std::move(layout)]
		(
			std::tuple<T*> outputs, 
			std::tuple<const std::complex<T>*> inputs, 
			std::tuple<>
		)
		{
			run_elementwise_loop(
				[] (T* result, const std::complex<T>* x)
				{
					store(result, std::abs(load(x)));
				},
				layout,
				std::get<ops::abs_operation::OUTPUT_OPERAND_RESULT>(outputs),
				std::get<ops::abs_operation::INPUT_OPERAND_VALUE>(inputs)
			);
		},
		type_list<T>(),
		type_list<std::complex<T>>()
	);
}

XMIPP4_NORETURN
std::shared_ptr<program> make_abs_program(
	joint_layout /*layout*/,
	type_list<void> /*types*/
)
{
	throw std::invalid_argument(
		"abs_program_builder::build: Expected arithmetic type."
	);
}

} // anonymous namespace

operation_id
abs_program_builder::get_operation_id() const noexcept
{
	return operation_id::of<ops::abs_operation>();
}

std::shared_ptr<xmipp4::program> abs_program_builder::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	if (!dynamic_cast<const ops::abs_operation*>(&operation))
	{
		throw std::invalid_argument(
			"abs_program_builder::build: Expected operation to "
			"be an instance of ops::abs_operation."
		);
	}

	if (
		output_signatures.size() !=
		ops::abs_operation::OUTPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"abs_program_builder::build: Expected exactly 1 "
			"output signature."
		);
	}

	if (
		input_signatures.size() !=
		ops::abs_operation::INPUT_OPERAND_COUNT
	)
	{
		throw std::invalid_argument(
			"abs_program_builder::build: Expected exactly 1 "
			"input signature."
		);
	}

	const auto &result_descriptor =
		output_signatures[ops::abs_operation::OUTPUT_OPERAND_RESULT]
		.get_descriptor();
	const auto &x_descriptor =
		input_signatures[ops::abs_operation::INPUT_OPERAND_VALUE]
		.get_descriptor();

	const auto data_type = x_descriptor.get_data_type();
	if (result_descriptor.get_data_type() != make_real(data_type))
	{
		throw std::invalid_argument(
			"abs_program_builder::build: Input operand must match output "
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
			return make_abs_program(
				std::move(layout),
				type_list<type>()
			);
		},
		native_arithmetic_type_map(),
		data_type
	);
}

} // namespace cpu
} // namespace xmipp4
