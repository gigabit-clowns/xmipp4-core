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
 * @file host_unified_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of default_host_unified_buffer.hpp
 * @date 2024-11-26
 * 
 */

#include <xmipp4/core/compute/host/host_unified_buffer.hpp>

namespace xmipp4
{
namespace compute
{

host_unified_buffer* host_unified_buffer::get_device_accessible_alias() noexcept
{
    return this;
}

const host_unified_buffer* 
host_unified_buffer::get_device_accessible_alias() const noexcept
{
    return this;
}

host_unified_buffer* host_unified_buffer::get_host_accessible_alias() noexcept
{
    return this;
}

const host_unified_buffer* 
host_unified_buffer::get_host_accessible_alias() const noexcept
{
    return this;
}

} // namespace compute
} // namespace xmipp4
