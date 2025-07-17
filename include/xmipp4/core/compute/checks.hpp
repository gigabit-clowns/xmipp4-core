// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "numerical_type.hpp"
#include "copy_region.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Check that both buffer sizes are equal.
 * 
 * 
 * @param count1 First buffer's size.
 * @param count2 Second buffer's size.
 * @return std::size_t The common size.
 * @throws std::invalid_argument if unequal.
 * 
 */
XMIPP4_CORE_API
std::size_t require_same_buffer_size(std::size_t size1, std::size_t size2);

/**
 * @brief Check that the source copy region is within bounds.
 * 
 * @param region Region to be copied.
 * @param count Element count in the source buffer.
 * @throws std::out_of_range if out of range.
 * 
 */
XMIPP4_CORE_API
void require_valid_source_region(const copy_region &region, std::size_t count);

/**
 * @brief Check that the destination copy region is within bounds.
 * 
 * @param region Region to be copied.
 * @param count Element count in the destination buffer.
 * @throws std::out_of_range if out of range.
 * 
 */
XMIPP4_CORE_API
void require_valid_destination_region(const copy_region &region, std::size_t count);

/**
 * @brief Check that copy region is within bounds.
 * 
 * @param region Region to be copied.
 * @param src_count Element count in the source buffer.
 * @param dst_count Element count in the destination buffer.
 * @throws std::out_of_range if out of range.
 * 
 */
XMIPP4_CORE_API
void require_valid_region(const copy_region &region, 
                          std::size_t src_count, 
                          std::size_t dst_count );

} // namespace compute
} // namespace xmipp4
