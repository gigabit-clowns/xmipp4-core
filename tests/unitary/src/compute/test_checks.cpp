// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <xmipp4/core/compute/checks.hpp>

using namespace xmipp4;
using namespace xmipp4::compute;

TEST_CASE( "require_same_buffer_size should return the common size", "[checks]" )
{
    std::size_t result;
    REQUIRE_NOTHROW(result = require_same_buffer_size(2, 2) );
    REQUIRE( result == 2 );
}

TEST_CASE( "require_same_buffer_size should throw when provided different sizes", "[checks]" )
{
    numerical_type result;
    REQUIRE_THROWS_AS( require_same_buffer_size(2, 3), std::invalid_argument );
    REQUIRE_THROWS_WITH( require_same_buffer_size(2, 3), "Both buffers must have the same size" );
}

TEST_CASE( "require_valid_source_region succeeds when provided correct parameters", "[checks]" )
{
    const copy_region region(10, 15, 10);
    REQUIRE_NOTHROW( require_valid_source_region(region, 20) );
}

TEST_CASE( "require_valid_source_region throws when provided out of bounds region", "[checks]" )
{
    const copy_region region(10, 15, 10);
    REQUIRE_THROWS_AS( require_valid_source_region(region, 19), std::out_of_range );
    REQUIRE_THROWS_WITH( require_valid_source_region(region, 19), "Source region must be within buffer bounds" );
}

TEST_CASE( "require_valid_destination_region succeeds when provided correct parameters", "[checks]" )
{
    const copy_region region(15, 10, 10);
    REQUIRE_NOTHROW( require_valid_destination_region(region, 20) );
}

TEST_CASE( "require_valid_destination_region throws when provided out of bounds region", "[checks]" )
{
    const copy_region region(15, 10, 10);
    REQUIRE_THROWS_AS( require_valid_destination_region(region, 19), std::out_of_range );
    REQUIRE_THROWS_WITH( require_valid_destination_region(region, 19), "Destination region must be within buffer bounds" );
}
