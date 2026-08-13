// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/parametric_operation.hpp>
#include <xmipp4/ops/ops_component.hpp>
#include <xmipp4/ops/policies/reduction_operation_shape_policy.hpp>
#include <xmipp4/ops/rules/operand_type_rules.hpp>

namespace xmipp4
{
namespace ops
{

/**
 * @brief Add the elements of the input array along the reduced axes.
 *
 * Every type add accepts is admitted, including boolean, whose sum is
 * a disjunction. Overflow is a question about values rather than about
 * types, and is not one a domain can answer.
 */
XMIPP4_DECLARE_PARAMETRIC_OPERATION(
	sum,
	ops_component,
	XMIPP4_OPERANDS("result"),
	XMIPP4_OPERANDS("value"),
	reduction_operation_shape_policy,
	unary_homogeneous_rule<>
);

} // namespace ops
} // namespace xmipp4
