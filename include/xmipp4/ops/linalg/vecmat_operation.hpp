// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/basic_operation.hpp>
#include <xmipp4/ops/ops_component.hpp>
#include <xmipp4/ops/policies/vector_matrix_shape_policy.hpp>
#include <xmipp4/ops/rules/operand_type_rules.hpp>

namespace xmipp4
{
namespace ops
{

/**
 * @brief Multiply a stack of vectors by a stack of matrices.
 *
 * The vector's last axis is contracted with the matrix's second to last
 * axis, and everything before is a stack broadcast between the operands.
 * The mirror image of matvec: neither operand is promoted, so the vector
 * must already have rank one or more and the matrix rank two or more.
 *
 * @see matvec
 * @see matmul
 */
XMIPP4_DECLARE_OPERATION(
	vecmat,
	ops_component,
	XMIPP4_OPERANDS("result"),
	XMIPP4_OPERANDS("left", "right"),
	vector_matrix_shape_policy,
	binary_homogeneous_rule<arithmetic_type_domain>
);

} // namespace ops
} // namespace xmipp4
