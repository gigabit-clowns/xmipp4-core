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
 * @file test_communicator_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for communicator_manager.hpp
 * @date 2024-10-29
 */

#include <xmipp4/core/communication/communicator_manager.hpp>

#include <xmipp4/core/exceptions/ambiguous_backend_error.hpp>
#include <xmipp4/core/version.hpp>

#include <algorithm>

#include "mock/mock_communicator_backend.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "register communicator backend", "[communicator_manager]" ) 
{
    // Setup mocks
    auto mock1 = std::make_unique<mock_communicator_backend>();
    const std::string name1 = "mock1";
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name1)
        .TIMES(1);

    auto mock2 = std::make_unique<mock_communicator_backend>();
    const std::string name2 = "mock2";
    REQUIRE_CALL(*mock2, get_name())
        .RETURN(name2)
        .TIMES(1);

    // Test
    communicator_manager manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));

    std::vector<std::string> backends;
    manager.enumerate_backends(backends);

    REQUIRE( backends.size() == 2 );
    std::sort(backends.begin(), backends.end()); // Ordering not defined
    REQUIRE( backends[0] == name1 );
    REQUIRE( backends[1] == name2 );
}

TEST_CASE( "query communicator backend", "[communicator_manager]" ) 
{
    // Setup mocks
    auto mock1 = std::make_unique<mock_communicator_backend>();
    const std::string name1 = "mock1";
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name1)
        .TIMES(2);

    auto mock2 = std::make_unique<mock_communicator_backend>();
    const std::string name2 = "mock2";
    REQUIRE_CALL(*mock2, get_name())
        .RETURN(name2)
        .TIMES(2);

    // Test
    communicator_manager manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));

    communicator_backend* backend;
    backend = manager.get_backend(name1);
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == name1 );

    backend = manager.get_backend(name2);
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == name2 );

    REQUIRE( manager.get_backend("not-a-backend") == nullptr );
}

TEST_CASE( "register the same communicator backend twice", "[communicator_manager]" ) 
{
    // Setup mocks
    const std::string name = "mock1";
    auto mock1 = std::make_unique<mock_communicator_backend>();
    REQUIRE_CALL(*mock1, get_name())
        .RETURN(name)
        .TIMES(1);

    auto mock2 = std::make_unique<mock_communicator_backend>();
    REQUIRE_CALL(*mock2, get_name())
        .RETURN(name)
        .TIMES(1);

    // Test
    communicator_manager manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));

    std::vector<std::string> backends;
    manager.enumerate_backends(backends);

    REQUIRE( backends.size() == 1 );
    REQUIRE( backends[0] == name );
}

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
        .TIMES(2);

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
        .TIMES(2);

    communicator_manager manager;
    manager.register_backend(std::move(mock1));
    manager.register_backend(std::move(mock2));

    const std::string message = 
        "Could not disambiguate among multiple "
        "communicator_backend-s. Ensure that only one has "
        "been installed.";
    REQUIRE_THROWS_AS( manager.get_preferred_backend(), ambiguous_backend_error );
    REQUIRE_THROWS_WITH( manager.get_preferred_backend(), message );
}


