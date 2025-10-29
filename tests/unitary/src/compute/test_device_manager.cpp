// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/device_manager.hpp>

#include <xmipp4/core/compute/device.hpp>
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
