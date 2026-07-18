// SPDX-License-Identifier: GPL-3.0-only

#include "elementwise_program_builder.hpp"

#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/layout/joint_layout.hpp>
#include <xmipp4/core/layout/joint_layout_builder.hpp>
#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/platform/constexpr.hpp>

#include <backends/cpu/hardware/functor_program.hpp>
#include <backends/cpu/loops/elementwise_loop.hpp>

#include <array>
#include <cstddef>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace xmipp4
{
namespace cpu
{

namespace detail
{

/**
 * @brief Adapts a per-element kernel to the functor_program interface.
 *
 * Stores the kernel and the joint layout, and on invocation unpacks the
 * operand pointer tuples and drives run_elementwise_loop, passing the output
 * pointers followed by the input pointers to the kernel.
 */
template <typename F, typename Outputs, typename Inputs>
class elementwise_loop_functor;

template <typename F, typename... Outs, typename... Ins>
class elementwise_loop_functor<F, type_list<Outs...>, type_list<Ins...>>
{
public:
	elementwise_loop_functor(F functor, joint_layout layout)
		: m_functor(std::move(functor))
		, m_layout(std::move(layout))
	{
	}

	void operator()(
		std::tuple<Outs*...> outputs,
		std::tuple<const Ins*...> inputs,
		std::tuple<>
	) const
	{
		run(
			outputs,
			inputs,
			std::index_sequence_for<Outs...>(),
			std::index_sequence_for<Ins...>()
		);
	}

private:
	template <std::size_t... OutputIndices, std::size_t... InputIndices>
	void run(
		const std::tuple<Outs*...> &outputs,
		const std::tuple<const Ins*...> &inputs,
		std::index_sequence<OutputIndices...>,
		std::index_sequence<InputIndices...>
	) const
	{
		run_elementwise_loop(
			m_functor,
			m_layout,
			std::get<OutputIndices>(outputs)...,
			std::get<InputIndices>(inputs)...
		);
	}

	XMIPP4_NO_UNIQUE_ADDRESS F m_functor;
	joint_layout m_layout;
};

} // namespace detail

template <typename Op, typename KernelFactory, typename TypeDispatcher>
operation_id
elementwise_program_builder<Op, KernelFactory, TypeDispatcher>
::get_operation_id() const noexcept
{
	return operation_id::of<Op>();
}

template <typename Op, typename KernelFactory, typename TypeDispatcher>
std::shared_ptr<xmipp4::program>
elementwise_program_builder<Op, KernelFactory, TypeDispatcher>::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	XMIPP4_CONST_CONSTEXPR std::size_t output_count =
		static_cast<std::size_t>(Op::OUTPUT_OPERAND_COUNT);
	XMIPP4_CONST_CONSTEXPR std::size_t input_count =
		static_cast<std::size_t>(Op::INPUT_OPERAND_COUNT);

	const auto *typed_operation = dynamic_cast<const Op*>(&operation);
	if (typed_operation == nullptr)
	{
		throw std::invalid_argument(
			"elementwise_program_builder::build: Unexpected operation type."
		);
	}

	if (output_signatures.size() != output_count)
	{
		throw std::invalid_argument(
			"elementwise_program_builder::build: Unexpected output signature "
			"count."
		);
	}
	if (input_signatures.size() != input_count)
	{
		throw std::invalid_argument(
			"elementwise_program_builder::build: Unexpected input signature "
			"count."
		);
	}

	std::array<numerical_type, output_count> output_types;
	for (std::size_t i = 0; i < output_count; ++i)
	{
		output_types[i] = output_signatures[i].get_data_type();
	}
	std::array<numerical_type, input_count> input_types;
	for (std::size_t i = 0; i < input_count; ++i)
	{
		input_types[i] = input_signatures[i].get_data_type();
	}

	joint_layout_builder layout_builder;
	for (const auto &signature : output_signatures)
	{
		layout_builder.add_operand(signature.get_layout());
	}
	for (const auto &signature : input_signatures)
	{
		layout_builder.add_operand(signature.get_layout());
	}

	auto layout = layout_builder.build();
	const auto &factory = m_kernel_factory;
	return m_type_dispatcher.dispatch(
		[typed_operation, &layout, &factory]
		(auto output_element_types, auto input_element_types)
		{
			auto kernel = factory(
				*typed_operation,
				output_element_types,
				input_element_types
			);
			using loop_functor_type = detail::elementwise_loop_functor<
				decltype(kernel),
				decltype(output_element_types),
				decltype(input_element_types)
			>;
			return make_functor_program(
				loop_functor_type(std::move(kernel), std::move(layout)),
				output_element_types,
				input_element_types
			);
		},
		output_types,
		input_types
	);
}

} // namespace cpu
} // namespace xmipp4
