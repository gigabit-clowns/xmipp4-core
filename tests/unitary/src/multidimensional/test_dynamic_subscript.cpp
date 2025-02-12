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

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/dynamic_layout.hpp>

#include <type_traits>
#include <sstream>

using namespace xmipp4::multidimensional;


TEST_CASE("construct dynamic subscript from ellipsis_tag", "[dynamic_subscript]")
{
    const auto s = dynamic_subscript(ellipsis());
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::ellipsis );
}

TEST_CASE("construct dynamic subscript from new_axis_tag", "[dynamic_subscript]")
{
    const auto s = dynamic_subscript(new_axis());
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::new_axis );
}

TEST_CASE("construct dynamic subscript from index", "[dynamic_subscript]")
{
    const auto index = 1234;
    const auto s = dynamic_subscript(index);
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::index );
    REQUIRE( s.get_index() == index );
}

TEST_CASE("construct dynamic subscript from constant index", "[dynamic_subscript]")
{
    const auto index = std::integral_constant<int, 1234>();
    const auto s = dynamic_subscript(index);
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::index );
    REQUIRE( s.get_index() == index );
}

TEST_CASE("construct dynamic subscript from slice", "[dynamic_subscript]")
{
    const auto slice = make_slice(1, 2, 3);
    const auto s = dynamic_subscript(slice);
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::slice );
    REQUIRE( s.get_slice() == slice );
}

TEST_CASE("construct dynamic subscript from special slice", "[dynamic_subscript]")
{
    const auto s = dynamic_subscript(all());
    REQUIRE( s.get_subscript_type() == dynamic_subscript::subscript_type::slice );
    REQUIRE( s.get_slice() == all() );
}