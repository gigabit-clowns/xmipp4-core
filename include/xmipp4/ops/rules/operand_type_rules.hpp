// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/rules/operand_type_rule.hpp>
#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/numerical/numerical_type_domain_tags.hpp>

namespace xmipp4
{
namespace ops
{

/**
 * @defgroup operand_type_rules Common operand type rules
 *
 * The typing rules the operation catalog is built from. An operation names
 * one of these in its declaration and that is the whole of its typing: the
 * runtime policy and every backend type dispatcher read the same rule.
 *
 * Every rule is parameterised by the domain of its element type, which is
 * how an operation states what it accepts. A rule says nothing about
 * shapes, so the same rule serves an elementwise operation and a reduction.
 *
 * @{
 */

/**
 * @brief Unary operation whose operands share one element type.
 *
 * @tparam Domain The admissible element types.
 */
template <typename Domain = any_type_domain>
using unary_homogeneous_rule = operand_type_rule<
	type_list<pivot_from_input<0, Domain>>,
	type_list<slot_same_as<0>>,
	type_list<slot_same_as<0>>
>;

/**
 * @brief Binary operation whose operands share one element type.
 *
 * @tparam Domain The admissible element types.
 */
template <typename Domain = any_type_domain>
using binary_homogeneous_rule = operand_type_rule<
	type_list<pivot_from_input<0, Domain>>,
	type_list<slot_same_as<0>>,
	type_list<slot_same_as<0>, slot_same_as<0>>
>;

/**
 * @brief Ternary operation whose operands share one element type.
 *
 * @tparam Domain The admissible element types.
 */
template <typename Domain = any_type_domain>
using ternary_homogeneous_rule = operand_type_rule<
	type_list<pivot_from_input<0, Domain>>,
	type_list<slot_same_as<0>>,
	type_list<slot_same_as<0>, slot_same_as<0>, slot_same_as<0>>
>;

/**
 * @brief Unary operation producing the real counterpart of its input.
 *
 * Models operations that drop the imaginary part, such as `abs`, and leaves
 * non complex inputs untouched.
 *
 * @tparam Domain The admissible input element types.
 */
template <typename Domain = arithmetic_type_domain>
using unary_real_of_rule = operand_type_rule<
	type_list<pivot_from_input<0, Domain>>,
	type_list<slot_real_of<0>>,
	type_list<slot_same_as<0>>
>;

/**
 * @brief Unary operation producing the complex counterpart of its input.
 *
 * @tparam Domain The admissible input element types.
 */
template <typename Domain = inexact_type_domain>
using unary_complex_of_rule = operand_type_rule<
	type_list<pivot_from_input<0, Domain>>,
	type_list<slot_complex_of<0>>,
	type_list<slot_same_as<0>>
>;

/**
 * @brief Unary operation producing a boolean output.
 *
 * @tparam Domain The admissible input element types.
 */
template <typename Domain = any_type_domain>
using unary_predicate_rule = operand_type_rule<
	type_list<pivot_from_input<0, Domain>>,
	type_list<slot_fixed<numerical_type::boolean>>,
	type_list<slot_same_as<0>>
>;

/**
 * @brief Binary comparison producing a boolean output.
 *
 * @tparam Domain The admissible input element types.
 */
template <typename Domain = any_type_domain>
using binary_predicate_rule = operand_type_rule<
	type_list<pivot_from_input<0, Domain>>,
	type_list<slot_fixed<numerical_type::boolean>>,
	type_list<slot_same_as<0>, slot_same_as<0>>
>;

/**
 * @brief Nullary operation whose output element type is free.
 *
 * With no input to fix it, the element type is whatever the user supplied
 * output carries. Models operations that write a value into an existing
 * array, such as `fill`.
 *
 * @tparam Domain The admissible output element types.
 */
template <typename Domain = any_type_domain>
using nullary_free_rule = operand_type_rule<
	type_list<pivot_from_output<0, Domain>>,
	type_list<slot_same_as<0>>,
	type_list<>
>;

/**
 * @brief Unary operation whose output element type is independent.
 *
 * The output adopts the input element type unless the user pre-allocates an
 * output of a different one, which is what makes a converting copy possible
 * without a separate operation.
 *
 * @tparam OutputDomain The admissible output element types.
 * @tparam InputDomain The admissible input element types.
 */
template <
	typename OutputDomain = any_type_domain,
	typename InputDomain = any_type_domain
>
using converting_rule = operand_type_rule<
	type_list<
		pivot_from_output<0, OutputDomain, 1>,
		pivot_from_input<0, InputDomain>
	>,
	type_list<slot_same_as<0>>,
	type_list<slot_same_as<1>>
>;

/** @} */

} // namespace ops
} // namespace xmipp4
