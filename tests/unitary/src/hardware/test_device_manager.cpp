// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/hardware/device_manager.hpp>

#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/version.hpp>

#include <algorithm>

#include "mock/mock_device_backend.hpp"
#include "mock/mock_device.hpp"

using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE( "device_manager should allow registering backends", "[device_backend]" )
{
    device_manager manager;

    auto mock1 = std::make_unique<mock_device_backend>();
    REQUIRE_CALL(*mock1, get_name())
        .RETURN("mock1");
    auto mock2 = std::make_unique<mock_device_backend>();
    REQUIRE_CALL(*mock2, get_name())
        .RETURN("mock2");

    REQUIRE( manager.register_backend(std::move(mock1)) == true );
    REQUIRE( manager.register_backend(std::move(mock2)) == true );
}

TEST_CASE( "device_manager should not allow registering duplicate backends", "[device_backend]" )
{
    device_manager manager;

    auto mock = std::make_unique<mock_device_backend>();
    REQUIRE_CALL(*mock, get_name())
        .RETURN("mock");

    REQUIRE( manager.register_backend(std::move(mock)) == true );

    mock = std::make_unique<mock_device_backend>();
    REQUIRE_CALL(*mock, get_name())
        .RETURN("mock");
    REQUIRE( manager.register_backend(std::move(mock)) == false );
}

TEST_CASE( "device_manager should not allow registering null backend", "[device_backend]" )
{
    device_manager manager;

    REQUIRE( manager.register_backend( nullptr ) == false );
}

TEST_CASE( "enumerate devices in device_manager should list all items", "[device_manager]" ) 
{
    auto mock1 = std::make_unique<mock_device_backend>();
    const std::string name1 = "mock1";
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name1);
    REQUIRE_CALL(*mock1, enumerate_devices(ANY(std::vector<std::size_t>&)))
        .SIDE_EFFECT(_1 = {0, 1, 2, 3, 4});

    auto mock2 = std::make_unique<mock_device_backend>();
    const std::string name2 = "mock2";
    REQUIRE_CALL(*mock2, get_name())
        .RETURN(name2);
    REQUIRE_CALL(*mock2, enumerate_devices(ANY(std::vector<std::size_t>&)))
        .SIDE_EFFECT(_1 = {6, 7, 8, 9});

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

TEST_CASE( "create_device in device_manager should return the appropiate device", "[device_manager]" ) 
{
    const std::size_t device_id = 154433421;
    auto device = std::make_shared<mock_device>();
    auto mock1 = std::make_unique<mock_device_backend>();
    REQUIRE_CALL(*mock1, get_name())
        .RETURN("mock1");
    REQUIRE_CALL(*mock1, create_device(device_id))
        .RETURN(device);

    auto mock2 = std::make_unique<mock_device_backend>();
    REQUIRE_CALL(*mock2, get_name())
        .RETURN("mock2");

    device_manager manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));

    auto result = manager.create_device(device_index("mock1", device_id));
    REQUIRE( result == device );
}

TEST_CASE( "create_device in device_manager should throw when requesting an invalid device", "[device_manager]" ) 
{
    auto mock = std::make_unique<mock_device_backend>();
    REQUIRE_CALL(*mock, get_name())
        .RETURN("mock");

    device_manager manager;
    manager.register_backend(std::move(mock));

    REQUIRE_THROWS_MATCHES(
        manager.create_device(device_index("error", 0)),
		std::invalid_argument,
		Catch::Matchers::Message(
            "Requested backend does not exist"
        )
	);
}
