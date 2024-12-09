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
 * @file test_host_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for host_buffer.hpp
 * @date 2024-11-09
 * 
 */

#include <xmipp4/core/compute/host_buffer.hpp>
#include <xmipp4/core/compute/device_queue.hpp>

#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <trompeloeil.hpp>


using namespace xmipp4;
using namespace xmipp4::compute;

class mock_host_buffer final
    : public host_buffer
{
public:
    MAKE_MOCK0(get_size, std::size_t (), const noexcept override);
    MAKE_MOCK0(get_data, void* (), noexcept override);
    MAKE_CONST_MOCK0(get_data, const void* (), noexcept override);
    MAKE_MOCK0(get_device_accessible_alias, device_buffer* (), noexcept override);
    MAKE_CONST_MOCK0(get_device_accessible_alias, const device_buffer* (), noexcept override);
    MAKE_MOCK1(record_queue, void (device_queue&), override);

};



TEST_CASE( "get_device_accessible_alias should return null when null is provided", "[host_buffer]" )
{
    std::shared_ptr<host_buffer> buffer;
    REQUIRE( get_device_accessible_alias(buffer) == nullptr );
    std::shared_ptr<const host_buffer> const_buffer;
    REQUIRE( get_device_accessible_alias(const_buffer) == nullptr );
}

TEST_CASE( "get_device_accessible_alias should return null when buffer is not aliasable", "[host_buffer]" )
{
    auto mock = std::make_shared<mock_host_buffer>();

    REQUIRE_CALL(*mock, get_device_accessible_alias())
        .RETURN(nullptr)
        .TIMES(1);
    std::shared_ptr<host_buffer> buffer = mock;
    REQUIRE( get_device_accessible_alias(buffer) == nullptr );

    REQUIRE_CALL(const_cast<const mock_host_buffer&>(*mock), get_device_accessible_alias())
        .RETURN(nullptr)
        .TIMES(1);
    std::shared_ptr<const host_buffer> const_buffer = mock;
    REQUIRE( get_device_accessible_alias(const_buffer) == nullptr );
}

TEST_CASE( "get_device_accessible_alias should return the alias when buffer is aliasable", "[host_buffer]" )
{
    auto mock = std::make_shared<mock_host_buffer>();
    auto *alias = reinterpret_cast<device_buffer*>(std::uintptr_t(0xDEADBEEF)); 

    REQUIRE_CALL(*mock, get_device_accessible_alias())
        .RETURN(alias)
        .TIMES(1);
    std::shared_ptr<host_buffer> buffer = mock;
    REQUIRE( get_device_accessible_alias(buffer).get() == alias );

    REQUIRE_CALL(const_cast<const mock_host_buffer&>(*mock), get_device_accessible_alias())
        .RETURN(alias)
        .TIMES(1);
    std::shared_ptr<const host_buffer> const_buffer = mock;
    REQUIRE( get_device_accessible_alias(const_buffer).get() == alias );
}

TEST_CASE( "copy host buffer", "[host_buffer]" )
{
    const std::size_t n = 1024;

    std::vector<std::uint8_t> src_data(n);
    const void *src_ptr = src_data.data();
    mock_host_buffer src_buffer;
    REQUIRE_CALL(src_buffer, get_size())
        .RETURN(n)
        .TIMES(1);
    const mock_host_buffer &const_src_buffer = src_buffer;
    REQUIRE_CALL(const_src_buffer, get_data())
        .RETURN(src_ptr)
        .TIMES(1);

    std::vector<std::uint8_t> dst_data(n);
    void *dst_ptr = dst_data.data();
    mock_host_buffer dst_buffer;
    REQUIRE_CALL(dst_buffer, get_size())
        .RETURN(n)
        .TIMES(1);
    REQUIRE_CALL(dst_buffer, get_data())
        .RETURN(dst_ptr)
        .TIMES(1);

    // populate
    for (std::size_t i = 0; i < n; ++i)
    {
        src_data[i] = i*i + 2*i + 1;
    }

    copy(src_buffer, dst_buffer);

    // check
    for (std::size_t i = 0; i < n; ++i)
    {
        REQUIRE( dst_data[i] == src_data[i] );
    }
}

TEST_CASE( "copy host buffer with different size should throw", "[host_buffer]" )
{
    mock_host_buffer src;
    REQUIRE_CALL(src, get_size())
        .RETURN(8)
        .TIMES(2);

    mock_host_buffer dst;
    REQUIRE_CALL(dst, get_size())
        .RETURN(10)
        .TIMES(2);

    REQUIRE_THROWS_AS( copy(src, dst), std::invalid_argument ); 
    REQUIRE_THROWS_WITH( copy(src, dst), "Both buffers must have the same size" ); 
}

TEST_CASE( "copy host buffer regions", "[host_buffer]" )
{
    std::vector<std::uint8_t> src_data(1024);
    const void *src_ptr = src_data.data();
    mock_host_buffer src_buffer;
    REQUIRE_CALL(src_buffer, get_size())
        .RETURN(1024)
        .TIMES(1);
    const mock_host_buffer &const_src_buffer = src_buffer;
    REQUIRE_CALL(const_src_buffer, get_data())
        .RETURN(src_ptr)
        .TIMES(1);

    std::vector<std::uint8_t> dst_data(2048);
    void *dst_ptr = dst_data.data();
    mock_host_buffer dst_buffer;
    REQUIRE_CALL(dst_buffer, get_size())
        .RETURN(2048)
        .TIMES(1);
    REQUIRE_CALL(dst_buffer, get_data())
        .RETURN(dst_ptr)
        .TIMES(1);

    std::array<copy_region, 2> regions = {
        copy_region(512, 0, 512),
        copy_region(0, 1536, 512),
    };

    // populate
    for (std::size_t i = 0; i < 1024; ++i)
    {
        src_data[i] = 4*i*i + i + 2;
    }

    copy(src_buffer, dst_buffer, make_span(regions));

    // check
    for (std::size_t i = 0; i < 512; ++i)
    {
        REQUIRE( dst_data[i] == src_data[i+512] );
    }
    for (std::size_t i = 0; i < 512; ++i)
    {
        REQUIRE( dst_data[i+1536] == src_data[i] );
    }
}

TEST_CASE( "copy out of bounds host buffer regions", "[host_buffer]" )
{
    mock_host_buffer src;
    REQUIRE_CALL(src, get_size())
        .RETURN(1024)
        .TIMES(4);
    const mock_host_buffer &const_src = src;
    REQUIRE_CALL(const_src, get_data())
        .RETURN(nullptr)
        .TIMES(4);

    mock_host_buffer dst;
    REQUIRE_CALL(dst, get_size())
        .RETURN(512)
        .TIMES(4);
    REQUIRE_CALL(dst, get_data())
        .RETURN(nullptr)
        .TIMES(4);

    std::array<copy_region, 1> regions;
    
    regions = {
        copy_region(1024, 0, 1),
    };
    REQUIRE_THROWS_AS( copy(src, dst, make_span(regions)), std::out_of_range ); 
    REQUIRE_THROWS_WITH( copy(src, dst, make_span(regions)), "Source region must be within buffer bounds" ); 

    regions = {
        copy_region(0, 512, 1),
    };
    REQUIRE_THROWS_AS( copy(src, dst, make_span(regions)), std::out_of_range ); 
    REQUIRE_THROWS_WITH( copy(src, dst, make_span(regions)), "Destination region must be within buffer bounds" ); 
}
