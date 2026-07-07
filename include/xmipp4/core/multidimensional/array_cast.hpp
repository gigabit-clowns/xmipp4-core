// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "array_view.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class execution_context;

/**
 * @brief Cast an array to a given numerical type.
 *
 * When the array already has the requested type, an alias of the input is
 * returned without copying its data. Otherwise, a new array holding the
 * cast elements is allocated and returned.
 *
 * @param input The array to be cast.
 * @param target_type The numerical type to cast the array to.
 * @param context The execution context used for dispatching.
 * @return array The array cast to the target type.
 */
XMIPP4_CORE_API
array cast(
	array& input,
	numerical_type target_type,
	const execution_context &context
);

/**
 * @brief Cast an array to a given numerical type into a new array.
 *
 * Unlike @ref cast, this function always allocates a new array holding the
 * cast elements, even when the input already has the requested type.
 *
 * @param input The array to be cast.
 * @param target_type The numerical type to cast the array to.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The newly created array cast to the target type.
 */
XMIPP4_CORE_API
array cast_copy(
	array_view input,
	numerical_type target_type,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
