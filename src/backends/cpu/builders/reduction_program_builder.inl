// SPDX-License-Identifier: GPL-3.0-only

#include "reduction_program_builder.hpp"

#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/core/dispatch/operation_cast.hpp>
#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/platform/constexpr.hpp>

#include <backends/cpu/builders/dispatcher_support_query.hpp>
#include <backends/cpu/builders/reduction_layout_plan.hpp>
#include <backends/cpu/hardware/functor_program.hpp>
#include <backends/cpu/loops/reduction_loop.hpp>

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
 * @brief Adapts a reduction kernel to the functor_program interface.
 *
 * Stores the kernel and the planned iteration, and on invocation drives
 * run_reduction_loop with them.
 */
template <typename F, typename Outputs, typename Inputs>
class reduction_loop_functor;

template <typename F, typename... Outs, typename... Ins>
class reduction_loop_functor<F, type_list<Outs...>, type_list<Ins...>>
{
public:
	reduction_loop_functor(F functor, reduction_layout_plan plan)
		: m_functor(std::move(functor))
		, m_plan(std::move(plan))
	{
	}

	void operator()(
		std::tuple<Outs*...> outputs,
		std::tuple<const Ins*...> inputs,
		std::tuple<>
	) const
	{
		run_reduction_loop(
			m_functor,
			m_plan.get_kept_layout(),
			m_plan.get_reduced_layout(),
			m_plan.get_reduction_count(),
			outputs,
			inputs
		);
	}

private:
	XMIPP4_NO_UNIQUE_ADDRESS F m_functor;
	reduction_layout_plan m_plan;
};

/**
 * @brief Copy the data type of every operand signature into an array.
 *
 * @param types Destination, sized to the operand count.
 * @param signatures The operand signatures.
 */
template <std::size_t Count>
void extract_reduction_data_types(
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

template <
	typename Op,
	typename KernelFactory,
	typename TypeDispatcher,
	bool Ordered
>
operation_id
reduction_program_builder<Op, KernelFactory, TypeDispatcher, Ordered>
::get_operation_id() const noexcept
{
	return operation_id::of<Op>();
}

template <
	typename Op,
	typename KernelFactory,
	typename TypeDispatcher,
	bool Ordered
>
backend_priority
reduction_program_builder<Op, KernelFactory, TypeDispatcher, Ordered>
::get_suitability(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue &queue
) const
{
	XMIPP4_CONST_CONSTEXPR auto output_count =
		Op::output_operand_count;
	XMIPP4_CONST_CONSTEXPR auto input_count =
		Op::input_operand_count;

	const auto base = program_builder::get_suitability(
		operation,
		output_signatures,
		input_signatures,
		queue
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

	std::array<numerical_type, output_count> output_types;
	std::array<numerical_type, input_count> input_types;
	detail::extract_reduction_data_types(output_types, output_signatures);
	detail::extract_reduction_data_types(input_types, input_signatures);

	// Reporting an unsupported element type here, rather than throwing from
	// build(), is what lets the manager fall through to another backend.
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

template <
	typename Op,
	typename KernelFactory,
	typename TypeDispatcher,
	bool Ordered
>
std::shared_ptr<xmipp4::program>
reduction_program_builder<Op, KernelFactory, TypeDispatcher, Ordered>::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	XMIPP4_CONST_CONSTEXPR auto output_count =
		Op::output_operand_count;
	XMIPP4_CONST_CONSTEXPR auto input_count =
		Op::input_operand_count;

	const auto &typed_operation = operation_cast<Op>(operation);

	if (output_signatures.size() != output_count)
	{
		throw std::invalid_argument(
			"reduction_program_builder::build: Unexpected output signature "
			"count."
		);
	}
	if (input_signatures.size() != input_count)
	{
		throw std::invalid_argument(
			"reduction_program_builder::build: Unexpected input signature "
			"count."
		);
	}

	std::array<numerical_type, output_count> output_types;
	std::array<numerical_type, input_count> input_types;
	detail::extract_reduction_data_types(output_types, output_signatures);
	detail::extract_reduction_data_types(input_types, input_signatures);

	// The axes travel with the operation, so the whole partition of the
	// iteration space is settled here, once per program rather than once per
	// element.
	const auto &shape_policy = typed_operation.get_shape_policy();
	reduction_layout_plan plan(
		output_signatures,
		input_signatures,
		shape_policy.get_axes(),
		shape_policy.get_keep_dimensions(),
		Ordered
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
				input_element_types
			);
			using loop_functor_type = detail::reduction_loop_functor<
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
