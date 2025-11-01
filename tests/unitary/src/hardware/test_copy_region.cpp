// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/hardware/copy_region.hpp>

using namespace xmipp4::hardware;

TEST_CASE( "default constructed copy_region should have the expected values", "[copy_region]" )
{
    const copy_region region;
    REQUIRE( region.get_source_offset() == 0 );
    REQUIRE( region.get_destination_offset() == 0 );
    REQUIRE( region.get_count() == 0 );
}

TEST_CASE( "construct copy_region should produce the expected object", "[copy_region]" )
{
    const copy_region region(4, 6, 10);
    REQUIRE( region.get_source_offset() == 4 );
    REQUIRE( region.get_destination_offset() == 6 );
    REQUIRE( region.get_count() == 10 );
}
