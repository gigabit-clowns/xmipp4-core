#pragma once

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
 * @file aligned_alloc.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Allocate memory with alignment specifications.
 * @date 2024-12-02
 * 
 */

#include "../platform/attributes.hpp"

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
void* aligned_alloc(std::size_t size, std::size_t alignment) noexcept;

/**
 * @brief Release a memory block previously allocated with aligned_alloc
 * 
 * @param ptr Pointer to the data to be freed.
 */
void aligned_free(void* ptr) noexcept;

} // namespace memory
} // namespace xmipp4
