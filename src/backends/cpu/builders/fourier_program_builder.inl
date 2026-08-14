// SPDX-License-Identifier: GPL-3.0-only

#include "fourier_program_builder.hpp"

#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/core/dispatch/operation_cast.hpp>
#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>

#include <backends/cpu/builders/dispatcher_support_query.hpp>
#include <backends/cpu/builders/fourier_layout_plan.hpp>
#include <backends/cpu/hardware/functor_program.hpp>

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
 * @brief Adapts a Fourier transform to the functor_program interface.
 *
 * Stores the transform and the planned iteration, and on invocation displaces
 * each operand pointer by its offset and hands the two over. The offsets are
 * applied here, once and in one place, so that everything downstream describes
 * an array starting where it is pointed at.
 */
template <typename Transform, typename Outputs, typename Inputs>
class fourier_transform_functor;

template <typename Transform, typename Out, typename In>
class fourier_transform_functor<Transform, type_list<Out>, type_list<In>>
{
public:
	fourier_transform_functor(Transform transform, fourier_layout_plan plan)
		: m_transform(std::move(transform))
		, m_plan(std::move(plan))
	{
	}

	void operator()(
		std::tuple<Out*> outputs,
		std::tuple<const In*> inputs,
		std::tuple<>
	) const
	{
		m_transform(
			m_plan,
			std::get<0>(outputs) + m_plan.get_output_offset(),
			std::get<0>(inputs) + m_plan.get_input_offset()
		);
	}

private:
	XMIPP4_NO_UNIQUE_ADDRESS Transform m_transform;
	fourier_layout_plan m_plan;
};

} // namespace detail

template <typename Op, typename Transform, typename TypeDispatcher>
operation_id
fourier_program_builder<Op, Transform, TypeDispatcher>
::get_operation_id() const noexcept
{
	return operation_id::of<Op>();
}

template <typename Op, typename Transform, typename TypeDispatcher>
backend_priority
fourier_program_builder<Op, Transform, TypeDispatcher>::get_suitability(
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

	if (output_signatures.size() != 1 || input_signatures.size() != 1)
	{
		return backend_priority::unsupported;
	}

	const std::array<numerical_type, 1> output_types = {
		output_signatures[0].get_data_type()
	};
	const std::array<numerical_type, 1> input_types = {
		input_signatures[0].get_data_type()
	};

	// Reporting an unsupported element type here, rather than throwing from
	// build(), is what lets the manager fall through to another backend. Half
	// precision reaches this and is turned away, the transform having no
	// arithmetic of that width to compute in.
	const auto supported =
		detail::dispatcher_support_query<TypeDispatcher>::is_supported(
			make_span(output_types.data(), output_types.size()),
			make_span(input_types.data(), input_types.size())
		);
	if (!supported)
	{
		return backend_priority::unsupported;
	}

	return base;
}

template <typename Op, typename Transform, typename TypeDispatcher>
std::shared_ptr<xmipp4::program>
fourier_program_builder<Op, Transform, TypeDispatcher>::build(
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
			"fourier_program_builder::build: Unexpected output signature "
			"count."
		);
	}
	if (input_signatures.size() != 1)
	{
		throw std::invalid_argument(
			"fourier_program_builder::build: Unexpected input signature "
			"count."
		);
	}

	const std::array<numerical_type, 1> output_types = {
		output_signatures[0].get_data_type()
	};
	const std::array<numerical_type, 1> input_types = {
		input_signatures[0].get_data_type()
	};

	// The axes travel with the operation, so the whole description of the
	// transform is settled here, once per program rather than once per run.
	fourier_layout_plan plan(
		output_signatures[0],
		input_signatures[0],
		typed_operation.get_shape_policy().get_axes(),
		Transform::get_kind()
	);

	const auto &transform = m_transform;
	return m_type_dispatcher.dispatch(
		Op::get_static_descriptor(),
		[&plan, &transform]
		(auto output_element_types, auto input_element_types)
		{
			using loop_functor_type = detail::fourier_transform_functor<
				Transform,
				decltype(output_element_types),
				decltype(input_element_types)
			>;
			return make_functor_program(
				loop_functor_type(transform, std::move(plan)),
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
