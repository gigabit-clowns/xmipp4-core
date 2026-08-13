// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/parametric_operation.hpp>
#include <xmipp4/ops/ops_component.hpp>
#include <xmipp4/ops/policies/real_fourier_transform_shape_policy.hpp>
#include <xmipp4/ops/rules/operand_type_rules.hpp>

namespace xmipp4
{
namespace ops
{

/**
 * @brief Transform a real array to the frequency domain.
 *
 * The spectrum of a real signal is conjugate symmetric, so only the half
 * that is not redundant is stored: the last transformed axis holds
 * n / 2 + 1 coefficients, and every other keeps its extent.
 *
 * Complex operands are not admitted. Their spectra have no symmetry to
 * exploit, and storing half of one would lose information.
 */
XMIPP4_DECLARE_PARAMETRIC_OPERATION(
	rfft,
	ops_component,
	XMIPP4_OPERANDS("spectrum"),
	XMIPP4_OPERANDS("signal"),
	real_fourier_transform_shape_policy,
	unary_complex_of_rule<floating_point_type_domain>
);

} // namespace ops
} // namespace xmipp4
