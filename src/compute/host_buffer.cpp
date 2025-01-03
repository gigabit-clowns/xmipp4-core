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

#include <xmipp4/core/compute/checks.hpp>
#include <xmipp4/core/memory/align.hpp>

#include <stdexcept>
#include <cstring>

namespace xmipp4
{
namespace compute
{

template <typename DstBuffer, typename SrcBuffer>
static
std::shared_ptr<DstBuffer> 
get_device_accessible_alias_impl(const std::shared_ptr<SrcBuffer> &buffer) noexcept
{
    std::shared_ptr<DstBuffer> result;

    if (buffer)
    {
        auto *alias = buffer->get_device_accessible_alias();
        if (alias)
        {
            result = std::shared_ptr<DstBuffer>(buffer, alias);
        }
    }

    return result;
}



std::shared_ptr<device_buffer> 
get_device_accessible_alias(const std::shared_ptr<host_buffer> &buffer) noexcept
{
    return get_device_accessible_alias_impl<device_buffer>(buffer);
}

std::shared_ptr<const device_buffer> 
get_device_accessible_alias(const std::shared_ptr<const host_buffer> &buffer) noexcept
{
    return get_device_accessible_alias_impl<const device_buffer>(buffer);
}

void copy(const host_buffer &src_buffer, host_buffer &dst_buffer)
{
    const auto count = require_same_buffer_size(
        src_buffer.get_size(), dst_buffer.get_size()
    );
    std::memcpy(
        dst_buffer.get_data(),
        src_buffer.get_data(),
        count
    );
}

void copy(const host_buffer &src_buffer, host_buffer &dst_buffer,
          span<const copy_region> regions )
{
    const auto* src_data = src_buffer.get_data();
    auto* dst_data = dst_buffer.get_data();
    const auto src_count = src_buffer.get_size();
    const auto dst_count = dst_buffer.get_size();
    for (const copy_region &region : regions)
    {
        require_valid_region(region, src_count, dst_count);
        std::memcpy(
            memory::offset_bytes(dst_data, region.get_destination_offset()),
            memory::offset_bytes(src_data, region.get_source_offset()),
            region.get_count()
        );
    }
}

} // namespace compute
} // namespace xmipp4
