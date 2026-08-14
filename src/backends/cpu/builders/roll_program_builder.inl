// SPDX-License-Identifier: GPL-3.0-only

#include "roll_program_builder.hpp"

#include <backends/cpu/builders/dispatcher_support_query.hpp>
#include <backends/cpu/builders/roll_block_plan.hpp>
#include <backends/cpu/builders/type_dispatchers/rule_type_dispatcher.hpp>
#include <backends/cpu/hardware/functor_program.hpp>
#include <backends/cpu/loops/elementwise_loop.hpp>

#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/core/dispatch/operation_cast.hpp>
#include <xmipp4/core/layout/strided_layout.hpp>
#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/platform/constexpr.hpp>

#include <array>
#include <cstddef>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

namespace xmipp4
{
namespace cpu
{

namespace detail
{

/**
 * @brief Copy one element from an input pointer to an output one.
 *
 * A cyclic shift moves elements without converting them, and
 * unary_homogeneous_rule<> guarantees the output and input share their
 * element type, so there is nothing to do beyond the assignment.
 */
template <typename T>
struct roll_assign
{
	void operator()(T *out, const T *in) const noexcept
	{
		*out = *in;
	}
};

/**
 * @brief Per-element kernel for a cyclic axis shift.
 *
 * Drives run_elementwise_loop once per block precomputed by
 * build_roll_blocks, each a plain affine copy; the wraparound lives
 * entirely in how those blocks were carved out, not in this loop.
 */
template <typename T>
class roll_kernel
{
public:
	explicit roll_kernel(std::vector<joint_layout> blocks)
		: m_blocks(std::move(blocks))
	{
	}

	void operator()(
		std::tuple<T *> outputs,
		std::tuple<const T *> inputs,
		std::tuple<>
	) const
	{
		T *const out = std::get<0>(outputs);
		const T *const in = std::get<0>(inputs);
		for (const auto &block : m_blocks)
		{
			run_elementwise_loop(roll_assign<T>(), block, out, in);
		}
	}

private:
	std::vector<joint_layout> m_blocks;
};

} // namespace detail

template <typename Op, typename ShiftPolicy>
operation_id
roll_program_builder<Op, ShiftPolicy>::get_operation_id() const noexcept
{
	return operation_id::of<Op>();
}

template <typename Op, typename ShiftPolicy>
backend_priority
roll_program_builder<Op, ShiftPolicy>::get_suitability(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue &queue
) const
{
	const auto base = program_builder::get_suitability(
		operation, output_signatures, input_signatures, queue
	);
	if (base == backend_priority::unsupported)
	{
		return base;
	}

	if (output_signatures.size() != Op::output_operand_count ||
	    input_signatures.size() != Op::input_operand_count)
	{
		return backend_priority::unsupported;
	}

	const std::array<numerical_type, 1> output_types = {
		output_signatures[0].get_data_type()
	};
	const std::array<numerical_type, 1> input_types = {
		input_signatures[0].get_data_type()
	};

	using type_dispatcher_type = rule_type_dispatcher<typename Op::type_rule>;
	const auto supported =
		detail::dispatcher_support_query<type_dispatcher_type>::is_supported(
			make_span(output_types.data(), output_types.size()),
			make_span(input_types.data(), input_types.size())
		);
	if (!supported)
	{
		return backend_priority::unsupported;
	}

	return base;
}

template <typename Op, typename ShiftPolicy>
std::shared_ptr<xmipp4::program>
roll_program_builder<Op, ShiftPolicy>::build(
	const operation &operation,
	span<const operand_signature> output_signatures,
	span<const operand_signature> input_signatures,
	xmipp4::command_queue& /*queue*/,
	program_cache* /*cache*/
) const
{
	if (output_signatures.size() != Op::output_operand_count)
	{
		throw std::invalid_argument(
			"roll_program_builder::build: Unexpected output signature count."
		);
	}
	if (input_signatures.size() != Op::input_operand_count)
	{
		throw std::invalid_argument(
			"roll_program_builder::build: Unexpected input signature count."
		);
	}

	const auto &typed_operation = operation_cast<Op>(operation);
	const auto axes = typed_operation.get_shape_policy().get_axes();

	const auto &output_layout = output_signatures[0].get_layout();
	const auto &input_layout = input_signatures[0].get_layout();

	std::vector<std::size_t> extents;
	output_layout.get_extents(extents);
	std::vector<std::ptrdiff_t> output_strides;
	output_layout.get_strides(output_strides);
	std::vector<std::ptrdiff_t> input_strides;
	input_layout.get_strides(input_strides);

	std::vector<roll_axis_shift> shifts;
	shifts.reserve(axes.size());
	for (const auto axis : axes)
	{
		const auto shift = ShiftPolicy::shift_amount(extents[axis]);
		if (shift > 0)
		{
			shifts.emplace_back(axis, shift);
		}
	}

	auto blocks = build_roll_blocks(
		make_span(extents),
		make_span(output_strides),
		output_layout.get_offset(),
		make_span(input_strides),
		input_layout.get_offset(),
		make_span(shifts)
	);

	const std::array<numerical_type, 1> output_types = {
		output_signatures[0].get_data_type()
	};
	const std::array<numerical_type, 1> input_types = {
		input_signatures[0].get_data_type()
	};

	using type_dispatcher_type = rule_type_dispatcher<typename Op::type_rule>;
	return type_dispatcher_type::dispatch(
		Op::get_static_descriptor(),
		[&blocks]
		(auto output_element_types, auto input_element_types)
		{
			using out_t = typename type_list_element<
				0, decltype(output_element_types)
			>::type;

			return make_functor_program(
				detail::roll_kernel<out_t>(std::move(blocks)),
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
