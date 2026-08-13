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
 * @brief Negate each element of the input array into the output array.
 *
 * Only types that can represent a negative value are admitted. Complex
 * types are left out, matching what the reference backend implements
 * rather than what negation is defined on.
 */
XMIPP4_DECLARE_OPERATION(
	negate,
	ops_component,
	XMIPP4_OPERANDS("result"),
	XMIPP4_OPERANDS("value"),
	elementwise_operation_shape_policy,
	unary_homogeneous_rule<
		domain_union<signed_integer_type_domain, floating_point_type_domain>
	>
);

} // namespace ops
} // namespace xmipp4
