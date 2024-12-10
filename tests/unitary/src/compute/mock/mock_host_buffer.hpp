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
 * @file mock_host_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Mock for host_buffer interface.
 * @date 2024-12-10
 */

#include <xmipp4/core/compute/host_buffer.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_host_buffer final
    : public host_buffer
{
public:
    MAKE_MOCK0(get_size, std::size_t (), const noexcept override);
    MAKE_MOCK0(get_data, void* (), noexcept override);
    MAKE_CONST_MOCK0(get_data, const void* (), noexcept override);
    MAKE_MOCK0(get_device_accessible_alias, device_buffer* (), noexcept override);
    MAKE_CONST_MOCK0(get_device_accessible_alias, const device_buffer* (), noexcept override);
    MAKE_MOCK1(record_queue, void (device_queue&), override);

};

} // namespace compute
} // namespace xmipp4
