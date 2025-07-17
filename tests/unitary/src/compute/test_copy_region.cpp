// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file test_copy_region.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for copy_region.hpp
 * @date 2024-11-11
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/compute/copy_region.hpp>

using namespace xmipp4::compute;

TEST_CASE( "construct copy region", "[copy_region]" )
{
    const copy_region region(4, 6, 10);
    REQUIRE( region.get_source_offset() == 4 );
    REQUIRE( region.get_destination_offset() == 6 );
    REQUIRE( region.get_count() == 10 );
}

TEST_CASE( "default construct copy region", "[copy_region]" )
{
    const copy_region region;
    REQUIRE( region.get_source_offset() == 0 );
    REQUIRE( region.get_destination_offset() == 0 );
    REQUIRE( region.get_count() == 0 );
}
