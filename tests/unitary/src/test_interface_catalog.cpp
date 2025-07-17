// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file test_interface_catalog.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for interface_catalog.hpp
 * @date 2024-10-28
 * 
 */

#include <xmipp4/core/interface_catalog.hpp>

#include "mock/mock_backend_manager.hpp"

#include <catch2/catch_test_macros.hpp>
#include <trompeloeil/matcher/any.hpp>

using namespace xmipp4;

TEST_CASE( "get_backend_manager should always return the same instance", "[interface_catalog]" ) 
{
    interface_catalog catalog(false);

    auto& manager1 = catalog.get_backend_manager<mock_backend_manager>();
    auto& manager2 = catalog.get_backend_manager<mock_backend_manager>();
    REQUIRE( &manager1 == &manager2 );
}
