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
 * @file test_device_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of tests for device_backend
 * @date 2024-11-12
 * 
 */

#include <compute/test_device_backend.hpp>

#include <xmipp4/core/compute/device_backend.hpp>
#include <xmipp4/core/compute/device.hpp>

#include <catch2/catch_test_macros.hpp>

namespace xmipp4
{
namespace compute
{
namespace testing
{

void test_device_backend(device_backend &backend, const std::string &name)
{
    SECTION( "query backend" )
    {
        REQUIRE_NOTHROW( backend.is_available() );
        REQUIRE( backend.get_name() == name );
    }

    SECTION( "obtain device properties" )
    {
        std::vector<std::size_t> ids;
        backend.enumerate_devices(ids);
        for(const auto &id : ids)
        {
            device_properties properties;
            REQUIRE( backend.get_device_properties(id, properties) );
            REQUIRE( properties.get_name() != "" );
            REQUIRE( properties.get_physical_location() != "" );
            REQUIRE( properties.get_total_memory_bytes() > 0 );
        }
    }

    SECTION( "create devices" )
    {
        std::vector<std::size_t> ids;
        backend.enumerate_devices(ids);
        for(const auto &id : ids)
        {
            REQUIRE( backend.create_device(id) != nullptr );
            REQUIRE( backend.create_device_shared(id) != nullptr );
        }
    }
}

} // namespace testing
} // namespace compute
} // namespace xmipp4

