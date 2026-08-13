// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/basic_operation.hpp>
#include <xmipp4/ops/ops_component.hpp>
#include <xmipp4/ops/policies/dot_product_shape_policy.hpp>
#include <xmipp4/ops/rules/operand_type_rules.hpp>

namespace xmipp4
{
namespace ops
{

/**
 * @brief Contract the last axis of one array with an axis of another.
 *
 * Sums the products over the last axis of the first operand and the
 * second to last of the second, keeping every other axis of both. Two
 * vectors therefore give a scalar.
 *
 * Agrees with matmul for one and two dimensional operands and parts
 * company beyond that, keeping the leading axes rather than broadcasting
 * them as a stack.
 */
XMIPP4_DECLARE_OPERATION(
	dot,
	ops_component,
	XMIPP4_OPERANDS("result"),
	XMIPP4_OPERANDS("left", "right"),
	dot_product_shape_policy,
	binary_homogeneous_rule<arithmetic_type_domain>
);

} // namespace ops
} // namespace xmipp4
