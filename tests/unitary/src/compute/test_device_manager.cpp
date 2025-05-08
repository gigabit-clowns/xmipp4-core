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
 * @file test_device_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for device_manager.hpp
 * @date 2024-10-29
 */

#include <xmipp4/core/compute/device_manager.hpp>

#include <xmipp4/core/compute/device.hpp>
#include <xmipp4/core/compute/device_create_parameters.hpp>
#include <xmipp4/core/version.hpp>

#include <algorithm>

#include "mock/mock_device_backend.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace xmipp4;
using namespace xmipp4::compute;

TEST_CASE( "enumerate devices", "[device_manager]" ) 
{
    // Setup mocks
    auto mock1 = std::make_unique<mock_device_backend>();
    const std::string name1 = "mock1";
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name1)
        .TIMES(1);
    REQUIRE_CALL(*mock1, enumerate_devices(ANY(std::vector<std::size_t>&)))
        .SIDE_EFFECT(_1 = {0, 1, 2, 3, 4})   
        .TIMES(1);

    auto mock2 = std::make_unique<mock_device_backend>();
    const std::string name2 = "mock2";
    REQUIRE_CALL(*mock2, get_name())
        .RETURN(name2)
        .TIMES(1);
    REQUIRE_CALL(*mock2, enumerate_devices(ANY(std::vector<std::size_t>&)))
        .SIDE_EFFECT(_1 = {6, 7, 8, 9})
        .TIMES(1);

    // Test
    device_manager manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));
    
    std::vector<device_index> devices;
    manager.enumerate_devices(devices);
    std::sort(devices.begin(), devices.end()); // Ordering not defined. 

    REQUIRE( devices.size() == 9 );
    REQUIRE( devices[0] == device_index(name1, 0) );
    REQUIRE( devices[1] == device_index(name1, 1) );
    REQUIRE( devices[2] == device_index(name1, 2) );
    REQUIRE( devices[3] == device_index(name1, 3) );
    REQUIRE( devices[4] == device_index(name1, 4) );

    REQUIRE( devices[5] == device_index(name2, 6) );
    REQUIRE( devices[6] == device_index(name2, 7) );
    REQUIRE( devices[7] == device_index(name2, 8) );
    REQUIRE( devices[8] == device_index(name2, 9) );
}
