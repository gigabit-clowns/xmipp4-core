// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/parametric_operation.hpp>
#include <xmipp4/ops/ops_component.hpp>
#include <xmipp4/ops/policies/inverse_real_fourier_transform_shape_policy.hpp>
#include <xmipp4/ops/rules/operand_type_rules.hpp>

namespace xmipp4
{
namespace ops
{

/**
 * @brief Transform a half spectrum back into a real array.
 *
 * The inverse of rfft. The extent of the restored axis is recovered from
 * the parity the operation carries, an axis of m coefficients coming
 * from either 2*(m - 1) or 2*m - 1 samples.
 */
XMIPP4_DECLARE_PARAMETRIC_OPERATION(
	irfft,
	ops_component,
	XMIPP4_OPERANDS("signal"),
	XMIPP4_OPERANDS("spectrum"),
	inverse_real_fourier_transform_shape_policy,
	unary_real_of_rule<complex_type_domain>
);

} // namespace ops
} // namespace xmipp4
