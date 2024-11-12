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
 * @file checks.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of checks.hpp
 * @date 2024-11-12
 * 
 */

#include <xmipp4/core/compute/checks.hpp>

#include <stdexcept>

namespace xmipp4 
{
namespace compute
{

numerical_type require_same_type(numerical_type type1, numerical_type type2)
{
    if (type1 != type2)
    {
        throw std::invalid_argument(
            "All buffers must have the same numerical type"
        );
    }
    return type1;
}

std::size_t require_same_count(std::size_t count1, std::size_t count2)
{
    if (count1 != count2)
    {
        throw std::invalid_argument(
            "All buffers must have the same element count"
        );
    }
    return count1;
}

void require_valid_source_region(const copy_region &region, std::size_t count)
{
    if ((region.get_source_offset() + region.get_count()) > count)
    {
        throw std::invalid_argument(
            "Source region must be within buffer bounds"
        );
    }
}

void require_valid_destination_region(const copy_region &region, std::size_t count)
{
    if ((region.get_destination_offset() + region.get_count()) > count)
    {
        throw std::invalid_argument(
            "Destination region must be within buffer bounds"
        );
    }
}

void require_valid_region(const copy_region &region, 
                          std::size_t src_count, 
                          std::size_t dst_count )
{
    require_valid_source_region(region, src_count);
    require_valid_destination_region(region, dst_count);
}

} // namespace compute
} // namespace xmipp4
