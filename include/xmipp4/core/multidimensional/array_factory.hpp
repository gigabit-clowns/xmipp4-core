// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"

namespace xmipp4 
{
namespace hardware
{

class device_queue;
class memory_allocator;
class buffer;

} // namespace hardware
namespace multidimensional
{

/**
 * @brief Create an empty array with given layout and data type.
 * 
 * @param layout The layout of the resulting array.
 * @param data_type The data type of the resulting array.
 * @param allocator Allocator from which to allocate the storage.
 * @param queue Optional device_queue where the allocation is asynchronously
 * dispatched.
 * @param out Optional array to reuse. If provided, its components may be 
 * cannibalized to create the resulting array. After the call, the out array
 * will be in a valid but unspecified state.
 * @return array The resulting empty array.
 */
XMIPP4_NODISCARD XMIPP4_CORE_API
array empty(
	strided_layout layout, 
	numerical_type data_type,
	hardware::memory_allocator &allocator,
	hardware::device_queue *queue,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4


