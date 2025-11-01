// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/communication/communicator_manager.hpp>

#include <xmipp4/core/exceptions/ambiguous_backend_error.hpp>
#include <xmipp4/core/version.hpp>

#include <algorithm>

#include "mock/mock_communicator_backend.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "get_preferred_backend should return the best available backend", "[communicator_manager]" ) 
{
    auto mock1 = std::make_unique<mock_communicator_backend>();
    const std::string name1 = "mock1";
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name1)
        .TIMES(2);
    REQUIRE_CALL(*mock1, get_priority())
        .RETURN(backend_priority::normal)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(*mock1, is_available())
        .RETURN(true)
        .TIMES(1);

    auto mock2 = std::make_unique<mock_communicator_backend>();
    const std::string name2 = "mock2";
    REQUIRE_CALL(*mock2, get_name())
        .RETURN(name2)
        .TIMES(1);
    REQUIRE_CALL(*mock2, get_priority())
        .RETURN(backend_priority::fallback)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(*mock2, is_available())
        .RETURN(true)
        .TIMES(1);

    auto mock3 = std::make_unique<mock_communicator_backend>();
    const std::string name3 = "mock3";
    REQUIRE_CALL(*mock3, get_name())
        .RETURN(name3)
        .TIMES(1);
    REQUIRE_CALL(*mock3, is_available())
        .RETURN(false)
        .TIMES(1);

    communicator_manager manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));
    manager.register_backend(std::move(mock3));

    const auto* backend = manager.get_preferred_backend();
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == name1 );
}

TEST_CASE( "get_preferred_backend should return null with no available backends", "[communicator_manager]" ) 
{
    auto mock1 = std::make_unique<mock_communicator_backend>();
    const std::string name1 = "mock1";
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name1)
        .TIMES(1);
    REQUIRE_CALL(*mock1, is_available())
        .RETURN(false)
        .TIMES(1);

    communicator_manager manager;
    manager.register_backend(std::move(mock1));

    const auto* backend = manager.get_preferred_backend();
    REQUIRE( backend == nullptr );
}

TEST_CASE( "get_preferred_backend should throw with multiple backends with same priority", "[communicator_manager]" ) 
{
    auto mock1 = std::make_unique<mock_communicator_backend>();
    const std::string name1 = "mock1";
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name1)
        .TIMES(1);
    REQUIRE_CALL(*mock1, get_priority())
        .RETURN(backend_priority::normal)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(*mock1, is_available())
        .RETURN(true)
        .TIMES(1);

    auto mock2 = std::make_unique<mock_communicator_backend>();
    const std::string name2 = "mock2";
    REQUIRE_CALL(*mock2, get_name())
        .RETURN(name2)
        .TIMES(1);
    REQUIRE_CALL(*mock2, get_priority())
        .RETURN(backend_priority::normal)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(*mock2, is_available())
        .RETURN(true)
        .TIMES(1);

    communicator_manager manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));

    const std::string message = 
        "Could not disambiguate among multiple "
        "communicator_backend-s. Ensure that only one has "
        "been installed.";
	REQUIRE_THROWS_MATCHES( 
		manager.get_preferred_backend(),
		ambiguous_backend_error,
		Catch::Matchers::Message(message)
	);
}
