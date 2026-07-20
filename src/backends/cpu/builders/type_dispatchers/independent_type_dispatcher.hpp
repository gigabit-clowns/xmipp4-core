// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/platform/attributes.hpp>

#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

namespace xmipp4
{

class program;

namespace cpu
{

/**
 * @brief Predicate accepting every element type combination.
 *
 * Default predicate of independent_type_dispatcher. Named after its
 * dispatcher, as each dispatcher defines the default predicate matching its
 * own predicate shape.
 */
template <typename... Types>
struct independent_always_supported : std::true_type {};

/**
 * @brief Type dispatcher resolving each operand's numerical type
 * independently.
 *
 * Resolves the numerical type of every output and input operand to a
 * compile-time element type without requiring any relation between them,
 * and invokes the program factory with the resolved type_lists. The
 * @p Predicate, instantiated over all the element types in operand order
 * (outputs first, then inputs), gates admissibility: when it rejects a
 * combination the factory is not invoked (so its body is never instantiated
 * for the rejected combination) and an exception is thrown instead.
 *
 * @tparam Predicate Admissibility predicate, instantiated as
 * Predicate<OutputTypes..., InputTypes...>. Must expose a boolean `value`
 * member.
 *
 * @warning The factory is instantiated for the full cartesian product of
 * the operand element types: with N operands, up to 16^N instantiations.
 */
template <
	template <typename...> class Predicate = independent_always_supported
>
class independent_type_dispatcher
{
public:
	/**
	 * @brief Resolve the operand element types and invoke the factory.
	 *
	 * The operand counts are inferred from the extents of the type arrays.
	 *
	 * @param factory Program factory invoked as factory(output_types,
	 * input_types), where both arguments are type_list-s of the resolved
	 * element types.
	 * @param output_types Runtime numerical type of each output operand.
	 * @param input_types Runtime numerical type of each input operand.
	 * @return std::shared_ptr<xmipp4::program> The program built by the
	 * factory.
	 * @throws std::invalid_argument if the type combination is rejected by
	 * the predicate.
	 */
	template <typename F, std::size_t OutputCount, std::size_t InputCount>
	static std::shared_ptr<xmipp4::program> dispatch(
		F &&factory,
		const std::array<numerical_type, OutputCount> &output_types,
		const std::array<numerical_type, InputCount> &input_types
	);

private:
	template <
		typename F,
		std::size_t InputCount,
		std::size_t... OutputIndices
	>
	static std::shared_ptr<xmipp4::program> dispatch_output_types(
		F &&factory,
		const std::array<
			numerical_type,
			sizeof...(OutputIndices)
		> &output_types,
		const std::array<numerical_type, InputCount> &input_types,
		std::index_sequence<OutputIndices...> output_indices
	);

	template <typename F, typename... Outputs, std::size_t... InputIndices>
	static std::shared_ptr<xmipp4::program> dispatch_input_types(
		F &&factory,
		type_list<Outputs...> output_element_types,
		const std::array<
			numerical_type,
			sizeof...(InputIndices)
		> &input_types,
		std::index_sequence<InputIndices...> input_indices
	);

	template <typename F, typename Outputs, typename Inputs>
	static std::shared_ptr<xmipp4::program> invoke_if(
		std::true_type supported,
		F &&factory,
		Outputs output_types,
		Inputs input_types
	);

	template <typename F, typename Outputs, typename Inputs>
	XMIPP4_NORETURN
	static std::shared_ptr<xmipp4::program> invoke_if(
		std::false_type supported,
		F &&factory,
		Outputs output_types,
		Inputs input_types
	);
};

} // namespace cpu
} // namespace xmipp4

#include "independent_type_dispatcher.inl"
