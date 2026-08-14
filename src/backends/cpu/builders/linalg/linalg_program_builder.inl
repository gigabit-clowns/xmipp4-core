// SPDX-License-Identifier: GPL-3.0-only

#include "linalg_program_builder.hpp"

#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/core/dispatch/operation_cast.hpp>
#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/layout/strided_layout.hpp>
#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/platform/constexpr.hpp>

#include <backends/cpu/builders/dispatcher_support_query.hpp>
#include <backends/cpu/builders/linalg/linalg_core_layout_plan.hpp>
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
 * @brief Adapts a per-batch-element kernel to the functor_program interface.
 *
 * Stores the kernel and the planned batch/core split, and on invocation
 * drives run_elementwise_loop over the batch, handing the kernel one output
 * pointer and two input pointers per batch element.
 */
template <typename F, typename Outputs, typename Inputs>
class linalg_loop_functor;

template <typename F, typename Out, typename Left, typename Right>
class linalg_loop_functor<F, type_list<Out>, type_list<Left, Right>>
{
public:
	linalg_loop_functor(F functor, linalg_core_layout_plan plan)
		: m_functor(std::move(functor))
		, m_plan(std::move(plan))
	{
	}

	void operator()(
		std::tuple<Out*> outputs,
		std::tuple<const Left*, const Right*> inputs,
		std::tuple<>
	) const
	{
		run_elementwise_loop(
			m_functor,
			m_plan.get_batch_layout(),
			std::get<0>(outputs),
			std::get<0>(inputs),
			std::get<1>(inputs)
		);
	}

private:
	XMIPP4_NO_UNIQUE_ADDRESS F m_functor;
	linalg_core_layout_plan m_plan;
};

/**
 * @brief Copy the data type of every operand signature into an array.
 */
template <std::size_t Count>
void extract_linalg_data_types(
	std::array<numerical_type, Count> &types,
	span<const operand_signature> signatures
) noexcept
{
	for (std::size_t i = 0; i < Count; ++i)
	{
		types[i] = signatures[i].get_data_type();
	}
}

} // namespace detail

template <typename Op, typename KernelFactory, typename TypeDispatcher>
operation_id
linalg_program_builder<Op, KernelFactory, TypeDispatcher>
::get_operation_id() const noexcept
{
	return operation_id::of<Op>();
}

template <typename Op, typename KernelFactory, typename TypeDispatcher>
backend_priority
linalg_program_builder<Op, KernelFactory, TypeDispatcher>
::get_suitability(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue &queue
) const
{
	XMIPP4_CONST_CONSTEXPR auto output_count = Op::output_operand_count;
	XMIPP4_CONST_CONSTEXPR auto input_count = Op::input_operand_count;
	using core_ranks = linalg_core_ranks<Op>;

	const auto base = program_builder::get_suitability(
		operation, output_signatures, input_signatures, queue
	);
	if (base == backend_priority::unsupported)
	{
		return base;
	}

	if (output_signatures.size() != output_count ||
	    input_signatures.size() != input_count)
	{
		return backend_priority::unsupported;
	}

	// A rank smaller than the core it is asked to supply means the operand
	// was promoted by the shape policy (matmul's vector promotion): this
	// builder does not implement that, and declines rather than misreading
	// a batch axis as a core one.
	if (output_signatures[0].get_layout().get_rank() < core_ranks::output ||
	    input_signatures[0].get_layout().get_rank() < core_ranks::left ||
	    input_signatures[1].get_layout().get_rank() < core_ranks::right)
	{
		return backend_priority::unsupported;
	}

	std::array<numerical_type, output_count> output_types;
	std::array<numerical_type, input_count> input_types;
	detail::extract_linalg_data_types(output_types, output_signatures);
	detail::extract_linalg_data_types(input_types, input_signatures);

	const auto supported =
		detail::dispatcher_support_query<TypeDispatcher>::is_supported(
			make_span(output_types.data(), output_count),
			make_span(input_types.data(), input_count)
		);
	if (!supported)
	{
		return backend_priority::unsupported;
	}

	return base;
}

template <typename Op, typename KernelFactory, typename TypeDispatcher>
std::shared_ptr<xmipp4::program>
linalg_program_builder<Op, KernelFactory, TypeDispatcher>::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	XMIPP4_CONST_CONSTEXPR auto output_count = Op::output_operand_count;
	XMIPP4_CONST_CONSTEXPR auto input_count = Op::input_operand_count;
	using core_ranks = linalg_core_ranks<Op>;

	const auto &typed_operation = operation_cast<Op>(operation);

	if (output_signatures.size() != output_count)
	{
		throw std::invalid_argument(
			"linalg_program_builder::build: Unexpected output signature "
			"count."
		);
	}
	if (input_signatures.size() != input_count)
	{
		throw std::invalid_argument(
			"linalg_program_builder::build: Unexpected input signature "
			"count."
		);
	}

	std::array<numerical_type, output_count> output_types;
	std::array<numerical_type, input_count> input_types;
	detail::extract_linalg_data_types(output_types, output_signatures);
	detail::extract_linalg_data_types(input_types, input_signatures);

	auto plan = linalg_core_layout_plan::for_trailing_core(
		output_signatures,
		input_signatures,
		core_ranks::output,
		core_ranks::left,
		core_ranks::right
	);

	const auto &factory = m_kernel_factory;
	return m_type_dispatcher.dispatch(
		Op::get_static_descriptor(),
		[&typed_operation, &plan, &factory]
		(auto output_element_types, auto input_element_types)
		{
			auto kernel = factory(
				typed_operation,
				output_element_types,
				input_element_types,
				plan
			);
			using loop_functor_type = detail::linalg_loop_functor<
				decltype(kernel),
				decltype(output_element_types),
				decltype(input_element_types)
			>;
			return make_functor_program(
				loop_functor_type(std::move(kernel), std::move(plan)),
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
