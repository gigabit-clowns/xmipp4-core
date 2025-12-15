// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"

#include "../hardware/device_context.hpp"

namespace xmipp4 
{
namespace multidimensional
{


/**
 * @brief Create an empty array for a given descriptor.
 * 
 * @param descriptor The descriptor for the new array.
 * @param placement The placement for the array's data.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its storage may be aliased
 * in the new array, avoiding memory allocations.
 * @return array The resulting empty array.
 */
XMIPP4_NODISCARD XMIPP4_CORE_API
array empty(
	array_descriptor descriptor, 
	hardware::target_placement placement,
	const hardware::device_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
