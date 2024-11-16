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
    MAKE_MOCK(get_type, auto () -> numerical_type, const noexcept override);
    MAKE_MOCK(get_count, auto () -> std::size_t, const noexcept override);
    MAKE_MOCK(get_data, auto () -> void*, noexcept override);
    MAKE_CONST_MOCK(get_data, auto () -> const void*, noexcept override);

};



TEST_CASE( "copy host buffer", "[host_buffer]" )
{
    const std::size_t n = 1024;

    std::vector<std::uint32_t> src_data(n);
    const void* src_ptr = src_data.data();
    mock_host_buffer src_buffer;
    REQUIRE_CALL(src_buffer, get_count())
        .RETURN(n)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(src_buffer, get_type())
        .RETURN(numerical_type::int32)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(const_cast<const mock_host_buffer&>(src_buffer), get_data())
        .RETURN(src_ptr)
        .TIMES(AT_LEAST(1));

    std::vector<std::uint32_t> dst_data(n);
    void* dst_ptr = dst_data.data();
    mock_host_buffer dst_buffer;
    REQUIRE_CALL(dst_buffer, get_count())
        .RETURN(n)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(dst_buffer, get_type())
        .RETURN(numerical_type::int32)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(dst_buffer, get_data())
        .RETURN(dst_ptr)
        .TIMES(AT_LEAST(1));

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

TEST_CASE( "copy host buffer with different type", "[host_buffer]" )
{
    mock_host_buffer src;
    ALLOW_CALL(src, get_count())
        .RETURN(100);
    REQUIRE_CALL(src, get_type())
        .RETURN(numerical_type::int32)
        .TIMES(AT_LEAST(1));

    mock_host_buffer dst;
    ALLOW_CALL(dst, get_count())
        .RETURN(100);
    REQUIRE_CALL(dst, get_type())
        .RETURN(numerical_type::int16)
        .TIMES(AT_LEAST(1));

    REQUIRE_THROWS_AS( copy(src, dst), std::invalid_argument ); 
    REQUIRE_THROWS_WITH( copy(src, dst), "Both buffers must have the same numerical type" ); 
}

TEST_CASE( "copy host buffer with different size", "[host_buffer]" )
{
    mock_host_buffer src;
    REQUIRE_CALL(src, get_count())
        .RETURN(8)
        .TIMES(AT_LEAST(1));
    ALLOW_CALL(src, get_type())
        .RETURN(numerical_type::int32);

    mock_host_buffer dst;
    REQUIRE_CALL(dst, get_count())
        .RETURN(10)
        .TIMES(AT_LEAST(1));
    ALLOW_CALL(dst, get_type())
        .RETURN(numerical_type::int32);

    REQUIRE_THROWS_AS( copy(src, dst), std::invalid_argument ); 
    REQUIRE_THROWS_WITH( copy(src, dst), "Both buffers must have the same element count" ); 
}

TEST_CASE( "copy host buffer regions", "[host_buffer]" )
{
    std::vector<std::uint32_t> src_data(1024);
    const void* src_ptr = src_data.data();
    mock_host_buffer src_buffer;
    REQUIRE_CALL(src_buffer, get_count())
        .RETURN(1024)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(src_buffer, get_type())
        .RETURN(numerical_type::int32)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(const_cast<const mock_host_buffer&>(src_buffer), get_data())
        .RETURN(src_ptr)
        .TIMES(AT_LEAST(1));

    std::vector<std::uint32_t> dst_data(2048);
    void* dst_ptr = dst_data.data();
    mock_host_buffer dst_buffer;
    REQUIRE_CALL(dst_buffer, get_count())
        .RETURN(2048)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(dst_buffer, get_type())
        .RETURN(numerical_type::int32)
        .TIMES(AT_LEAST(1));
    REQUIRE_CALL(dst_buffer, get_data())
        .RETURN(dst_ptr)
        .TIMES(AT_LEAST(1));

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
    REQUIRE_CALL(src, get_count())
        .RETURN(1024)
        .TIMES(AT_LEAST(1));
    ALLOW_CALL(src, get_type())
        .RETURN(numerical_type::int32);
    ALLOW_CALL(const_cast<const mock_host_buffer&>(src), get_data())
        .RETURN(nullptr);

    mock_host_buffer dst;
    REQUIRE_CALL(dst, get_count())
        .RETURN(512)
        .TIMES(AT_LEAST(1));
    ALLOW_CALL(dst, get_type())
        .RETURN(numerical_type::int32);
    ALLOW_CALL(dst, get_data())
        .RETURN(nullptr);

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

TEST_CASE( "copy host buffer regions with different type", "[host_buffer]" )
{
    mock_host_buffer src;
    ALLOW_CALL(src, get_count())
        .RETURN(256);
    REQUIRE_CALL(src, get_type())
        .RETURN(numerical_type::int32)
        .TIMES(AT_LEAST(1));

    mock_host_buffer dst;
    ALLOW_CALL(dst, get_count())
        .RETURN(128);
    REQUIRE_CALL(dst, get_type())
        .RETURN(numerical_type::int16)
        .TIMES(AT_LEAST(1));

    std::array<copy_region, 1> regions = {
        copy_region(16, 32, 64),
    };

    REQUIRE_THROWS_AS( copy(src, dst, make_span(regions)), std::invalid_argument ); 
    REQUIRE_THROWS_WITH( copy(src, dst, make_span(regions)), "Both buffers must have the same numerical type" ); 
}
