// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file aligned_alloc.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of aligned_alloc.hpp
 * @date 2024-12-02
 * 
 */

#include <xmipp4/core/memory/aligned_alloc.hpp>

#include <xmipp4/core/platform/operating_system.h>

#include <cstdlib>

namespace xmipp4
{
namespace memory
{

void* aligned_alloc(std::size_t size, std::size_t alignment) noexcept
{
    #if XMIPP4_WINDOWS
        return _aligned_malloc(size, alignment);
    #else
        return std::aligned_alloc(alignment, size);
    #endif

}

void aligned_free(void* ptr) noexcept
{
    #if XMIPP4_WINDOWS
        return _aligned_free(ptr);
    #else
        return std::free(ptr);
    #endif
}



} // namespace memory
} // namespace xmipp4
