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


using namespace xmipp4;
using namespace xmipp4::compute;

class test_host_buffer final
    : public host_buffer
{
public:
    test_host_buffer(std::size_t n)
        : m_data(n)
    {
    }

    numerical_type get_type() const noexcept final
    {
        return numerical_type::int32;
    }

    std::size_t get_count() const noexcept final
    {
        return m_data.size();
    }

    void* get_data() noexcept final
    {
        return m_data.data();
    }

    const void* get_data() const noexcept final
    {
        return m_data.data();
    }

private:
    std::vector<std::int32_t> m_data;

};



TEST_CASE( "copy host buffer", "[host_buffer]" )
{
    const std::size_t n = 1024;
    test_host_buffer src(n);   
    test_host_buffer dst(n);
    auto* src_data = static_cast<std::uint32_t*>(src.get_data());
    auto* dst_data = static_cast<std::uint32_t*>(dst.get_data());

    // populate
    for (std::size_t i = 0; i < n; ++i)
    {
        src_data[i] = i*i + 2*i + 1;
    }

    copy(src, dst);

    // check
    for (std::size_t i = 0; i < n; ++i)
    {
        REQUIRE( dst_data[i] == src_data[i] );
    }
}

TEST_CASE( "copy host buffer with different size", "[host_buffer]" )
{
    test_host_buffer src(1024);   
    test_host_buffer dst(8);

    REQUIRE_THROWS_AS( copy(src, dst), std::invalid_argument ); 
    REQUIRE_THROWS_WITH( copy(src, dst), "Both buffers must have the same element count" ); 
}

TEST_CASE( "copy host buffer regions", "[host_buffer]" )
{
    test_host_buffer src(1024);   
    test_host_buffer dst(2048);
    auto* src_data = static_cast<std::uint32_t*>(src.get_data());
    auto* dst_data = static_cast<std::uint32_t*>(dst.get_data());
    std::array<copy_region, 2> regions = {
        copy_region(512, 0, 512),
        copy_region(0, 1536, 512),
    };

    // populate
    for (std::size_t i = 0; i < 1024; ++i)
    {
        src_data[i] = 4*i*i + i + 2;
    }

    copy(src, dst, make_span(regions));

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
    test_host_buffer src(1024);   
    test_host_buffer dst(512);

    std::array<copy_region, 1> regions;
    
    regions = {
        copy_region(1024, 0, 1),
    };
    REQUIRE_THROWS_AS( copy(src, dst, make_span(regions)), std::invalid_argument ); 
    REQUIRE_THROWS_WITH( copy(src, dst, make_span(regions)), "Source region is out of bounds" ); 

    regions = {
        copy_region(0, 512, 1),
    };
    REQUIRE_THROWS_AS( copy(src, dst, make_span(regions)), std::invalid_argument ); 
    REQUIRE_THROWS_WITH( copy(src, dst, make_span(regions)), "Destination region is out of bounds" ); 
}
