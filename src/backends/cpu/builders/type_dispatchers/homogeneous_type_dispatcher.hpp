// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/platform/attributes.hpp>

#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>

namespace xmipp4
{

class program;

namespace cpu
{

/**
 * @brief Predicate accepting every shared element type.
 *
 * Default predicate of homogeneous_type_dispatcher. Named after its
 * dispatcher, as each dispatcher defines the default predicate matching its
 * own predicate shape.
 */
template <typename T>
struct homogeneous_always_supported : std::true_type {};

/**
 * @brief Type dispatcher requiring all operands to share one numerical type.
 *
 * Validates that every output and input operand has the same numerical type
 * (using the first output type as reference), resolves that single type to a
 * compile-time element type and invokes the program factory with type_lists
 * that repeat it once per operand. The @p Predicate, instantiated over the
 * shared element type, gates admissibility: when it rejects the type the
 * factory is not invoked (so its body is never instantiated for the rejected
 * type) and an exception is thrown instead.
 *
 * @tparam Predicate Admissibility predicate, instantiated as Predicate<T>
 * over the shared element type. Must expose a boolean `value` member.
 */
template <template <typename> class Predicate = homogeneous_always_supported>
class homogeneous_type_dispatcher
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
	 * Must not be empty.
	 * @param input_types Runtime numerical type of each input operand.
	 * @return std::shared_ptr<xmipp4::program> The program built by the
	 * factory.
	 * @throws std::invalid_argument if the operand types are not homogeneous
	 * or the shared type is rejected by the predicate.
	 */
	template <typename F, std::size_t OutputCount, std::size_t InputCount>
	static std::shared_ptr<xmipp4::program> dispatch(
		F &&factory,
		const std::array<numerical_type, OutputCount> &output_types,
		const std::array<numerical_type, InputCount> &input_types
	);

private:
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

#include "homogeneous_type_dispatcher.inl"
