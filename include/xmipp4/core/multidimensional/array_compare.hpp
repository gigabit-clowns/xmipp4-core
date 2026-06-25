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
array less(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array less_equal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array greater(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array greater_equal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array equal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array inequal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
