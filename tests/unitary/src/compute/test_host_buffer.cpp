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

#include <xmipp4/core/compute/host/host_numerical_type.hpp>

#include <vector>

#include <catch2/catch_test_macros.hpp>


using namespace xmipp4::compute;

template <typename T>
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
        return host_numerical_type<T>::value();
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
    std::vector<T> m_data;

};



TEST_CASE( "copy host buffer", "[host_buffer]" )
{
    const std::size_t n = 1024;
    test_host_buffer<std::uint32_t> src(n);   
    test_host_buffer<std::uint32_t> dst(n);
    auto* src_data = static_cast<std::uint32_t*>(src.get_data());
    auto* dst_data = static_cast<std::uint32_t*>(dst.get_data());

    // populate
    for (std::size_t i = 0; i < n; ++i)
    {
        src_data[i] = i;
    }

    copy(src, dst);

    // check
    for (std::size_t i = 0; i < n; ++i)
    {
        REQUIRE( dst_data[i] == i );
    }
}

TEST_CASE( "copy host buffer with different size", "[host_buffer]" )
{
    test_host_buffer<std::uint32_t> src(1024);   
    test_host_buffer<std::uint32_t> dst(8);

    REQUIRE_THROWS( copy(src, dst) ); 
}

TEST_CASE( "copy host buffer with different type", "[host_buffer]" )
{
    test_host_buffer<std::uint32_t> src(8);   
    test_host_buffer<std::int32_t> dst(8);

    REQUIRE_THROWS( copy(src, dst) ); 
}