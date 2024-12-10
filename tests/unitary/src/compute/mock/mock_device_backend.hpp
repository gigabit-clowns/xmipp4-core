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
 * @file mock_device_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Mock for device_backend interface.
 * @date 2024-12-10
 */

#include <xmipp4/core/compute/device_backend.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_device_backend final
    : public device_backend
{
public:
    MAKE_MOCK0(get_name, std::string (), const noexcept override);
    MAKE_MOCK0(get_version, version (), const noexcept override);
    MAKE_MOCK0(is_available, bool (), const noexcept override);
    MAKE_MOCK0(get_priority, backend_priority (), const noexcept override);
    MAKE_MOCK1(enumerate_devices, void (std::vector<std::size_t>&), const override);
    MAKE_MOCK2(get_device_properties, bool (std::size_t, device_properties&), const override);
    MAKE_MOCK2(create_device, std::unique_ptr<device> (std::size_t, const device_create_parameters&), override);
    MAKE_MOCK2(create_device_shared, std::shared_ptr<device> (std::size_t, const device_create_parameters&), override);

};

} // namespace compute
} // namespace xmipp4
