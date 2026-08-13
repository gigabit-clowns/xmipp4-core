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
 * @brief Select between two arrays according to a condition.
 *
 * The element type follows the arrays being selected from and not
 * the condition, which is always boolean.
 */
XMIPP4_DECLARE_OPERATION(
	where,
	ops_component,
	XMIPP4_OPERANDS("result"),
	XMIPP4_OPERANDS("condition", "on_true", "on_false"),
	elementwise_operation_shape_policy,
	selection_rule<>
);

} // namespace ops
} // namespace xmipp4
