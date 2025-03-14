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
 * @file test_dynamic_subscript.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for dynamic_subscript.hpp
 * @date 2025-02-12
 * 
 */

#include <xmipp4/core/multidimensional/dynamic_layout.hpp>

#include "mock/mock_subscript_callable.hpp"

#include <type_traits>
#include <sstream>

#include <catch2/catch_test_macros.hpp>

using namespace xmipp4::multidimensional;



TEST_CASE("construct dynamic_subscript from ellipsis_tag", "[dynamic_subscript]")
{
    const auto s = dynamic_subscript(ellipsis());
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::ellipsis );
}

TEST_CASE("construct dynamic_subscript from new_axis_tag", "[dynamic_subscript]")
{
    const auto s = dynamic_subscript(new_axis());
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::new_axis );
}

TEST_CASE("construct dynamic_subscript from index", "[dynamic_subscript]")
{
    const auto index = 1234;
    const auto s = dynamic_subscript(index);
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::index );
    REQUIRE( s.get_index() == index );
}

TEST_CASE("construct dynamic_subscript from constant index", "[dynamic_subscript]")
{
    const auto index = std::integral_constant<int, 1234>();
    const auto s = dynamic_subscript(index);
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::index );
    REQUIRE( s.get_index() == index );
}

TEST_CASE("construct dynamic_subscript from slice", "[dynamic_subscript]")
{
    const auto slice = make_slice(1, 2, 3);
    const auto s = dynamic_subscript(slice);
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::slice );
    REQUIRE( s.get_slice() == slice );
}

TEST_CASE("construct dynamic_subscript from special slice", "[dynamic_subscript]")
{
    const auto s = dynamic_subscript(all());
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::slice );
    REQUIRE( s.get_slice() == all() );
}

TEST_CASE("copy-construct dynamic_subscript from slice", "[dynamic_subscript]")
{
    const auto slice = make_slice(1, 2, 3);
    const auto s0 = dynamic_subscript(slice);
    const dynamic_subscript s1(s0);
    REQUIRE( s1.get_subscript_type() == dynamic_subscript::subscript_type::slice );
    REQUIRE( s1.get_slice() == slice );
}

TEST_CASE("copy-assign dynamic_subscript from slice", "[dynamic_subscript]")
{
    const auto slice = make_slice(1, 2, 3);
    const auto s0 = dynamic_subscript(slice);
    auto s1 = dynamic_subscript(new_axis());
    s1 = s0;
    REQUIRE( s1.get_subscript_type() == dynamic_subscript::subscript_type::slice );
    REQUIRE( s1.get_slice() == slice );
}

TEST_CASE("visit function with a dynamic_subscript that holds an ellipsis_tag", "[dynamic_subscript]")
{
    mock_subscript_callable mock;
    REQUIRE_CALL(mock, function_call(ellipsis()));

    const auto s = dynamic_subscript(ellipsis());
    visit(mock, s);
}

TEST_CASE("visit function with a dynamic_subscript that holds a new_axis_tag", "[dynamic_subscript]")
{
    mock_subscript_callable mock;
    REQUIRE_CALL(mock, function_call(new_axis()));

    const auto s = dynamic_subscript(new_axis());
    visit(mock, s);
}

TEST_CASE("visit function with a dynamic_subscript that holds an index", "[dynamic_subscript]")
{
    mock_subscript_callable mock;
    const std::ptrdiff_t index = 12345;
    REQUIRE_CALL(mock, function_call(index));

    const auto s = dynamic_subscript(index);
    visit(mock, s);
}

TEST_CASE("visit function with a dynamic subscript that holds a slice", "[dynamic_subscript]")
{
    mock_subscript_callable mock;
    const auto slice = make_slice(1, 2, 3);
    REQUIRE_CALL(mock, function_call(slice));

    const auto s = dynamic_subscript(slice);
    visit(mock, s);
}
