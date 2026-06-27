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
array complex(
	array_view real,
	array_view imag,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array polar(
	array_view abs,
	array_view angle,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array real(array value);

XMIPP4_CORE_API
array imag(array value);

XMIPP4_CORE_API
array angle(
	array_view value,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array conj(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
