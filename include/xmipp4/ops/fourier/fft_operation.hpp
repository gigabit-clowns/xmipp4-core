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
 * @brief Transform an array to the frequency domain.
 *
 * The transform is n dimensional: it acts along a set of axes and leaves
 * the rest as batch dimensions, so one operation serves the one, two and
 * three dimensional cases alike.
 *
 * A real operand is admitted and produces the complex spectrum of the
 * same precision. The full spectrum is stored, redundant halves and all;
 * use rfft to store only what a real signal needs.
 */
XMIPP4_DECLARE_PARAMETRIC_OPERATION(
	fft,
	ops_component,
	XMIPP4_OPERANDS("spectrum"),
	XMIPP4_OPERANDS("signal"),
	axiswise_operation_shape_policy,
	unary_complex_of_rule<inexact_type_domain>
);

} // namespace ops
} // namespace xmipp4
