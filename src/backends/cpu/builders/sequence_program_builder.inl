// SPDX-License-Identifier: GPL-3.0-only

#include "sequence_program_builder.hpp"

#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/core/dispatch/operation_cast.hpp>
#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/platform/constexpr.hpp>

#include <backends/cpu/builders/dispatcher_support_query.hpp>
#include <backends/cpu/builders/sequence_layout_plan.hpp>
#include <backends/cpu/hardware/functor_program.hpp>
#include <backends/cpu/loops/sequence_loop.hpp>

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
 * @brief Adapts a sequence generator to the functor_program interface.
 *
 * Stores the generator and the planned traversal, and on invocation drives
 * run_sequence_loop with them.
 */
template <typename F, typename Outputs, typename Inputs>
class sequence_loop_functor;

template <typename F, typename Out>
class sequence_loop_functor<F, type_list<Out>, type_list<>>
{
public:
	sequence_loop_functor(F functor, sequence_layout_plan plan)
		: m_functor(std::move(functor))
		, m_plan(plan)
	{
	}

	void operator()(
		std::tuple<Out*> outputs,
		std::tuple<>,
		std::tuple<>
	) const
	{
		run_sequence_loop(
			m_functor,
			std::get<0>(outputs) + m_plan.get_offset(),
			m_plan.get_count(),
			m_plan.get_stride()
		);
	}

private:
	XMIPP4_NO_UNIQUE_ADDRESS F m_functor;
	sequence_layout_plan m_plan;
};

} // namespace detail

template <typename Op, typename KernelFactory, typename TypeDispatcher>
operation_id
sequence_program_builder<Op, KernelFactory, TypeDispatcher>
::get_operation_id() const noexcept
{
	return operation_id::of<Op>();
}

template <typename Op, typename KernelFactory, typename TypeDispatcher>
backend_priority
sequence_program_builder<Op, KernelFactory, TypeDispatcher>
::get_suitability(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue &queue
) const
{
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

	if (output_signatures.size() != 1 || !input_signatures.empty())
	{
		return backend_priority::unsupported;
	}

	// A rank other than one is not something this backend could do slower;
	// it is something the shape policy already rules out. Reporting it as
	// unsupported rather than throwing keeps that distinction, and lets the
	// manager fall through if another backend disagrees.
	if (output_signatures[0].get_layout().get_rank() != 1)
	{
		return backend_priority::unsupported;
	}

	const std::array<numerical_type, 1> output_types = {
		output_signatures[0].get_data_type()
	};

	// Reporting an unsupported element type here, rather than throwing from
	// build(), is what lets the manager fall through to another backend.
	const auto supported =
		detail::dispatcher_support_query<TypeDispatcher>::is_supported(
			make_span(output_types.data(), 1),
			span<const numerical_type>()
		);
	if (!supported)
	{
		return backend_priority::unsupported;
	}

	return base;
}

template <typename Op, typename KernelFactory, typename TypeDispatcher>
std::shared_ptr<xmipp4::program>
sequence_program_builder<Op, KernelFactory, TypeDispatcher>::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	const auto &typed_operation = operation_cast<Op>(operation);

	if (output_signatures.size() != 1)
	{
		throw std::invalid_argument(
			"sequence_program_builder::build: Unexpected output signature "
			"count."
		);
	}
	if (!input_signatures.empty())
	{
		throw std::invalid_argument(
			"sequence_program_builder::build: Unexpected input signature "
			"count."
		);
	}

	const std::array<numerical_type, 1> output_types = {
		output_signatures[0].get_data_type()
	};

	// The whole traversal is three numbers off the layout, so it is settled
	// here, once per program rather than once per element.
	const sequence_layout_plan plan(output_signatures[0].get_layout());

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
			using loop_functor_type = detail::sequence_loop_functor<
				decltype(kernel),
				decltype(output_element_types),
				decltype(input_element_types)
			>;
			return make_functor_program(
				loop_functor_type(std::move(kernel), plan),
				output_element_types,
				input_element_types
			);
		},
		output_types,
		std::array<numerical_type, 0>()
	);
}

} // namespace cpu
} // namespace xmipp4
