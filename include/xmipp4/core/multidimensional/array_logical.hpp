// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "array_view.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class execution_context;

XMIPP4_CORE_API
array logical_negate(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array logical_and(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array logical_or(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array select(
	array_view mask,
	array_view x,
	array_view y,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
