// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/backend_manager.hpp>

#include "mock/mock_backend.hpp"

#include <catch2/catch_test_macros.hpp>
#include <trompeloeil/matcher/any.hpp>

using namespace xmipp4;

TEST_CASE( "register backend", "[backend_manager]" ) 
{
    // Setup mocks
    auto mock1 = std::make_unique<mock_backend>();
    const std::string name1 = "mock1";
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name1)
        .TIMES(1);

    auto mock2 = std::make_unique<mock_backend>();
    const std::string name2 = "mock2";
    REQUIRE_CALL(*mock2, get_name())
        .RETURN(name2)
        .TIMES(1);

    // Test
    basic_backend_manager<backend> manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));

    std::vector<std::string> backends;
    manager.enumerate_backends(backends);

    REQUIRE( backends.size() == 2 );
    std::sort(backends.begin(), backends.end()); // Ordering not defined
    REQUIRE( backends[0] == name1 );
    REQUIRE( backends[1] == name2 );
}

TEST_CASE( "query device backend", "[backend_manager]" ) 
{
    // Setup mocks
    auto mock1 = std::make_unique<mock_backend>();
    const std::string name1 = "mock1";
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name1)
        .TIMES(2);

    auto mock2 = std::make_unique<mock_backend>();
    const std::string name2 = "mock2";
    REQUIRE_CALL(*mock2, get_name())
        .RETURN(name2)
        .TIMES(2);

    // Test
    basic_backend_manager<backend> manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));

    backend* backend;
    backend = manager.get_backend(name1);
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == name1 );

    backend = manager.get_backend(name2);
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == name2 );

    REQUIRE( manager.get_backend("not-a-backend") == nullptr );
}

TEST_CASE( "register the same device backend twice", "[backend_manager]" ) 
{
    // Setup mocks
    const std::string name = "mock";
    auto mock1 = std::make_unique<mock_backend>();
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name)
        .TIMES(1);

    auto mock2 = std::make_unique<mock_backend>();
    REQUIRE_CALL(*mock2, get_name())
        .RETURN(name)
        .TIMES(1);

    // Test
    basic_backend_manager<backend> manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));

    std::vector<std::string> backends;
    manager.enumerate_backends(backends);

    REQUIRE( backends.size() == 1 );
    REQUIRE( backends[0] == name );
}
