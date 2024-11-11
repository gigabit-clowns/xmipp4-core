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
 * @file host_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_buffer.hpp
 * @date 2024-11-09
 * 
 */

#include <xmipp4/core/compute/host_buffer.hpp>

#include <xmipp4/core/memory/align.hpp>

#include <stdexcept>
#include <cstring>

namespace xmipp4
{
namespace compute
{

void copy(const host_buffer &src_buffer, host_buffer &dst_buffer)
{
    if (src_buffer.get_type() != dst_buffer.get_type())
    {
        throw std::invalid_argument("Both buffers must have the same numerical type");
    }
    
    if (src_buffer.get_count() != dst_buffer.get_count())
    {
        throw std::invalid_argument("Both buffers must have the same element count");
    }

    const auto element_size = get_size(src_buffer.get_type());
    std::memcpy(
        dst_buffer.get_data(),
        src_buffer.get_data(),
        element_size*src_buffer.get_count()
    );
}

void copy(const host_buffer &src_buffer, host_buffer &dst_buffer,
          span<const copy_region> regions )
{
    if (src_buffer.get_type() != dst_buffer.get_type())
    {
        throw std::invalid_argument("Both buffers must have the same numerical type");
    }

    const auto* src_data = src_buffer.get_data();
    auto* dst_data = dst_buffer.get_data();
    const auto element_size = get_size(src_buffer.get_type());
    for (const copy_region &region : regions)
    {
        if (region.get_source_offset()+region.get_count() > src_buffer.get_count())
        {
            throw std::invalid_argument("Source region is out of bounds");
        }
        if (region.get_destination_offset()+region.get_count() > dst_buffer.get_count())
        {
            throw std::invalid_argument("Destination region is out of bounds");
        }

        const auto region_bytes = as_bytes(region, element_size);
        std::memcpy(
            memory::offset_bytes(dst_data, region_bytes.get_destination_offset()),
            memory::offset_bytes(src_data, region_bytes.get_source_offset()),
            region_bytes.get_count()
        );
    }
}

} // namespace compute
} // namespace xmipp4
