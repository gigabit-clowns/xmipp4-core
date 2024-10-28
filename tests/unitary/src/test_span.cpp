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
 * @file test_slice.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for slice.hpp
 * @date 2024-02-28
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/span.hpp>

#include <array>

using namespace xmipp4;

TEST_CASE( "default construct span", "[span]" )
{
    span<int, 0> empty_span;
    REQUIRE(empty_span.size() == 0);
    REQUIRE(empty_span.data() == nullptr);

    span<int> dynamic_span;
    REQUIRE(dynamic_span.size() == 0);
    REQUIRE(dynamic_span.data() == nullptr);
}

TEST_CASE( "construct span from data and size", "[span]" )
{
    std::array<int, 8> data;

    span<int, 8> static_span(data.data(), data.size());
    REQUIRE(static_span.size() == data.size());
    REQUIRE(static_span.data() == data.data());

    span<int> dynamic_span(data.data(), data.size());
    REQUIRE(dynamic_span.size() == data.size());
    REQUIRE(dynamic_span.data() == data.data());
}

TEST_CASE( "copy construct span", "[span]" )
{
    std::array<int, 8> data;

    span<int, 8> static_span(data.data(), data.size());
    auto static_span2 = static_span;
    REQUIRE(static_span2.size() == data.size());
    REQUIRE(static_span2.data() == data.data());

    span<int> dynamic_span(data.data(), data.size());
    auto dynamic_span2 = dynamic_span;
    REQUIRE(dynamic_span2.size() == data.size());
    REQUIRE(dynamic_span2.data() == data.data());
}
