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
 * @file test_device_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for device_buffer.hpp
 * @date 2024-11-09
 * 
 */

#include <xmipp4/core/compute/device_buffer.hpp>
#include <xmipp4/core/compute/device_queue.hpp>

#include <catch2/catch_test_macros.hpp>
#include <trompeloeil.hpp>


using namespace xmipp4;
using namespace xmipp4::compute;

class mock_device_buffer final
    : public device_buffer
{
public:
    MAKE_MOCK0(get_size, std::size_t (), const noexcept override);
    MAKE_MOCK0(get_host_accessible_alias, host_buffer* (), noexcept override);
    MAKE_CONST_MOCK0(get_host_accessible_alias, const host_buffer* (), noexcept override);
    MAKE_MOCK1(record_queue, void (device_queue&), override);

};



TEST_CASE( "get_host_accessible_alias should return null when null is provided", "[device_buffer]" )
{
    std::shared_ptr<device_buffer> buffer;
    REQUIRE( get_host_accessible_alias(buffer) == nullptr );
    std::shared_ptr<const device_buffer> const_buffer;
    REQUIRE( get_host_accessible_alias(const_buffer) == nullptr );
}

TEST_CASE( "get_host_accessible_alias should return null when buffer is not aliasable", "[device_buffer]" )
{
    auto mock = std::make_shared<mock_device_buffer>();

    REQUIRE_CALL(*mock, get_host_accessible_alias())
        .RETURN(nullptr)
        .TIMES(1);
    std::shared_ptr<device_buffer> buffer = mock;
    REQUIRE( get_host_accessible_alias(buffer) == nullptr );

    REQUIRE_CALL(static_cast<const mock_device_buffer&>(*mock), get_host_accessible_alias())
        .RETURN(nullptr)
        .TIMES(1);
    std::shared_ptr<const device_buffer> const_buffer = mock;
    REQUIRE( get_host_accessible_alias(const_buffer) == nullptr );
}

TEST_CASE( "get_host_accessible_alias should return the alias when buffer is aliasable", "[device_buffer]" )
{
    auto mock = std::make_shared<mock_device_buffer>();
    auto *alias = reinterpret_cast<host_buffer*>(std::uintptr_t(0xDEADBEEF)); 

    REQUIRE_CALL(*mock, get_host_accessible_alias())
        .RETURN(alias)
        .TIMES(1);
    std::shared_ptr<device_buffer> buffer = mock;
    REQUIRE( get_host_accessible_alias(buffer).get() == alias );

    REQUIRE_CALL(static_cast<const mock_device_buffer&>(*mock), get_host_accessible_alias())
        .RETURN(alias)
        .TIMES(1);
    std::shared_ptr<const device_buffer> const_buffer = mock;
    REQUIRE( get_host_accessible_alias(const_buffer).get() == alias );
}
