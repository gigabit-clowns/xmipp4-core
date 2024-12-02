/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

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
