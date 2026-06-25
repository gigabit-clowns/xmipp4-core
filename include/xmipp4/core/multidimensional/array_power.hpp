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
array pow(
	array_view base,
	array_view exponent,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array exp(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array log(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array square(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array sqrt(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
