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
array negate(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array abs(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array add(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
void add_inplace(
	array &inout,
	array_view x,
	const execution_context &context
);

XMIPP4_CORE_API
array subtract(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
void subtract_inplace(
	array &inout,
	array_view x,
	const execution_context &context
);

XMIPP4_CORE_API
array multiply(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
void multiply_inplace(
	array &inout,
	array_view x,
	const execution_context &context
);

XMIPP4_CORE_API
array divide(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
void divide_inplace(
	array &inout,
	array_view x,
	const execution_context &context
);

} // namespace multidimensional
} // namespace xmipp4
