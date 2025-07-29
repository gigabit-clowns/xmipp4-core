// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/compute/device_index.hpp>

#include <sstream>


using namespace xmipp4::compute;

TEST_CASE( "construct device index", "[device_index]" )
{
    device_index index("example-backend", 4);
    REQUIRE( index.get_backend_name() == "example-backend" );
    REQUIRE( index.get_device_id() == 4 );
}

TEST_CASE( "parse device index", "[device_index]" )
{
    device_index index;

    REQUIRE( parse_device_index("cuda:3", index) );
    REQUIRE( index.get_backend_name() == "cuda" );
    REQUIRE( index.get_device_id() == 3 );

    REQUIRE( parse_device_index("cuda", index) );
    REQUIRE( index.get_backend_name() == "cuda" );
    REQUIRE( index.get_device_id() == 0 );
}

TEST_CASE( "parse invalid device index", "[device_index]" )
{
    device_index index;
    REQUIRE( !parse_device_index("3:example", index) );
    REQUIRE( !parse_device_index(":3", index) );
    REQUIRE( !parse_device_index("test:", index) );
    REQUIRE( !parse_device_index(":", index) );
    REQUIRE( !parse_device_index("", index) );
}

TEST_CASE( "device index to ostream", "[device_index]" )
{
    std::ostringstream oss;
    const device_index index("example", 4);
    oss << index;
    REQUIRE( oss.str() == "example:4" );
}
