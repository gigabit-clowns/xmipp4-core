// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/compute/device_properties.hpp>

#include <array>

using namespace xmipp4::compute;

TEST_CASE( "setting and getting attributes in device_properties should preserve them", "[compute]" )
{
    device_properties prop;
    prop.set_name("XMIPP A1234 SuperFast");
    prop.set_type(device_type::gpu);
    prop.set_physical_location("My computer");
    prop.set_total_memory_bytes(0xDEADBEEF);

    REQUIRE( prop.get_name() == "XMIPP A1234 SuperFast" );
    REQUIRE( prop.get_type() == device_type::gpu );
    REQUIRE( prop.get_physical_location() == "My computer" );
    REQUIRE( prop.get_total_memory_bytes() == 0xDEADBEEF );
}
