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
array bitwise_negate(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array bitwise_and(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array bitwise_or(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array bitwise_xor(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
