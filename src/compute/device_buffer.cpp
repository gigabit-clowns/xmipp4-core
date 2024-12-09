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
 * @date 2024-12-08
 * 
 */

#include <xmipp4/core/compute/device_buffer.hpp>

namespace xmipp4
{
namespace compute
{

template <typename DstBuffer, typename SrcBuffer>
static
std::shared_ptr<DstBuffer> 
get_host_accessible_alias_impl(const std::shared_ptr<SrcBuffer> &buffer) noexcept
{
    std::shared_ptr<DstBuffer> result;

    if (buffer)
    {
        auto *alias = buffer->get_host_accessible_alias();
        if (alias)
        {
            result = std::shared_ptr<DstBuffer>(buffer, alias);
        }
    }

    return result;
}



std::shared_ptr<host_buffer> 
get_host_accessible_alias(const std::shared_ptr<device_buffer> &buffer) noexcept
{
    return get_host_accessible_alias_impl<host_buffer>(buffer);
}

std::shared_ptr<const host_buffer> 
get_host_accessible_alias(const std::shared_ptr<const device_buffer> &buffer) noexcept
{
    return get_host_accessible_alias_impl<const host_buffer>(buffer);
}

} // namespace compute
} // namespace xmipp4
