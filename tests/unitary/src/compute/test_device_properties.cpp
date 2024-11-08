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
 * @file test_device_properties.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for device_properties.hpp
 * @date 2024-10-25
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/compute/device_properties.hpp>

#include <array>

using namespace xmipp4::compute;

TEST_CASE( "set attributes in device properties", "[compute]" )
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
