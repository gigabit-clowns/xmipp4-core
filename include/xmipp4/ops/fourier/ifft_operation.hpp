// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/parametric_operation.hpp>
#include <xmipp4/ops/ops_component.hpp>
#include <xmipp4/ops/policies/axiswise_operation_shape_policy.hpp>
#include <xmipp4/ops/rules/operand_type_rules.hpp>

namespace xmipp4
{
namespace ops
{

/**
 * @brief Transform an array back from the frequency domain.
 *
 * The result stays complex. A spectrum that is known to come from a real
 * signal inverts through irfft instead, which both halves the storage
 * and returns a real array.
 */
XMIPP4_DECLARE_PARAMETRIC_OPERATION(
	ifft,
	ops_component,
	XMIPP4_OPERANDS("signal"),
	XMIPP4_OPERANDS("spectrum"),
	axiswise_operation_shape_policy,
	unary_homogeneous_rule<complex_type_domain>
);

} // namespace ops
} // namespace xmipp4
