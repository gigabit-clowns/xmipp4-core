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
 * @param placement The placement of the allocated array data.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its components may be 
 * cannibalized to create the resulting array. After the call, the out array
 * will be in a valid but unspecified state. In addition, the resulting array
 * may alias contents of this buffer. Thus, modifications to the output buffer 
 * after this call may have side-effects on the resulting array.
 * @return array The resulting empty array.
 */
XMIPP4_NODISCARD XMIPP4_CORE_API
array empty(
	array_descriptor descriptor, 
	hardware::target_placement placement,
	const hardware::device_context &context,
	array *out = nullptr
);

/**
 * @brief Create an empty array with given layout and data type.
 * 
 * @param layout The layout of the resulting array.
 * @param placement The placement of the allocated array data.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its components may be 
 * cannibalized to create the resulting array. After the call, the out array
 * will be in a valid but unspecified state. In addition, the resulting array
 * may alias contents of this buffer. Thus, modifications to the output buffer 
 * after this call may have side-effects on the resulting array.
 * @return array The resulting empty array.
 */
XMIPP4_NODISCARD XMIPP4_CORE_API
array empty(
	strided_layout layout, 
	numerical_type data_type,
	hardware::target_placement placement,
	const hardware::device_context &context,
	array *out = nullptr
);

/**
 * @brief Create an empty array with given layout and data type.
 * 
 * @param extents The extents of the resulting array.
 * @param data_type The data type of the resulting array.
 * @param placement The placement of the allocated array data.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its components may be 
 * cannibalized to create the resulting array. After the call, the out array
 * will be in a valid but unspecified state. In addition, the resulting array
 * may alias contents of this buffer. Thus, modifications to the output buffer 
 * after this call may have side-effects on the resulting array.
 * @return array The resulting empty array.
 */
XMIPP4_NODISCARD XMIPP4_CORE_API
array empty(
	span<const std::size_t> extents, 
	numerical_type data_type,
	hardware::target_placement placement,
	const hardware::device_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
