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
array cast(
	array& input,
	numerical_type target_type,
	const execution_context &context
);

XMIPP4_CORE_API
array cast_copy(
	array_view input,
	numerical_type target_type,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
