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
 * @brief Compute the cube root of each element of the input array.
 *
 * Unlike the square root this is defined for negative operands, and
 * unlike it there is no complex counterpart to fall back on.
 */
XMIPP4_DECLARE_OPERATION(
	cbrt,
	ops_component,
	XMIPP4_OPERANDS("result"),
	XMIPP4_OPERANDS("value"),
	elementwise_operation_shape_policy,
	unary_homogeneous_rule<floating_point_type_domain>
);

} // namespace ops
} // namespace xmipp4
