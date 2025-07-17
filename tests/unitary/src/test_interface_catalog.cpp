// SPDX-License-Identifier: GPL-3.0-only

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
