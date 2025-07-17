// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file aligned_alloc.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Allocate memory with alignment specifications.
 * @date 2024-12-02
 * 
 */

#include "../platform/attributes.hpp"
#include "../platform/dynamic_shared_object.h"

#include <cstddef>

namespace xmipp4
{
namespace memory
{

/**
 * @brief Allocate memory with an alignment specification.
 * 
 * @param size Number of bytes to allocate. 
 * It must be multiple of the alignment.
 * @param alignment The alignment of the data. Must be power of two.
 * @return void* The allocated memory area. nullptr on failure.
 */
XMIPP4_CORE_API
void* aligned_alloc(std::size_t size, std::size_t alignment) noexcept;

/**
 * @brief Release a memory block previously allocated with aligned_alloc
 * 
 * @param ptr Pointer to the data to be freed.
 */
XMIPP4_CORE_API
void aligned_free(void* ptr) noexcept;

} // namespace memory
} // namespace xmipp4
