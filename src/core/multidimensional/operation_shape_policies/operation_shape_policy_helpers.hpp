// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/span.hpp>

#include <vector>

namespace xmipp4
{
namespace multidimensional
{

void require_1d(
	const std::vector<std::size_t> &shape, 
	const char *context,
	const char *operand_name
);

void require_2d(
	const std::vector<std::size_t> &shape, 
	const char *context,
	const char *operand_name
);

} // namespace multidimensional
} // namespace xmipp4

#include "operation_shape_policy_helpers.inl"
