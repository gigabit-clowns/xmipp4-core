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
 * @file checks.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Declaration of precondition handling routines.
 * @date 2024-11-12
 * 
 */

#include "numerical_type.hpp"
#include "copy_region.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Check that both numerical types are equal.
 * 
 * @param type1 First numerical type.
 * @param type2 Second numerical type.
 * @return numerical_type The common value.
 * @throws std::invalid_argument if unequal.
 * 
 */
XMIPP4_CORE_API
numerical_type require_same_type(numerical_type type1, numerical_type type2);

/**
 * @brief Check that both counts are equal.
 * 
 * 
 * @param count1 First count.
 * @param count2 Second count.
 * @return std::size_t The common count.
 * @throws std::invalid_argument if unequal.
 * 
 */
XMIPP4_CORE_API
std::size_t require_same_count(std::size_t count1, std::size_t count2);

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
