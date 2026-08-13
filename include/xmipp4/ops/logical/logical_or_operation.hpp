// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/basic_operation.hpp>
#include <xmipp4/ops/ops_component.hpp>
#include <xmipp4/ops/policies/elementwise_operation_shape_policy.hpp>
#include <xmipp4/ops/rules/operand_type_rules.hpp>

namespace xmipp4
{
namespace ops
{

/**
 * @brief Compute the disjunction of two arrays.
 *
 * Operands of any type are admitted, an element being true when
 * it is non zero, and the result is boolean whatever they were.
 */
XMIPP4_DECLARE_OPERATION(
	logical_or,
	ops_component,
	XMIPP4_OPERANDS("result"),
	XMIPP4_OPERANDS("left", "right"),
	elementwise_operation_shape_policy,
	binary_predicate_rule<>
);

} // namespace ops
} // namespace xmipp4
