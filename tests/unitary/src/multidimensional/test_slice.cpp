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

#include <xmipp4/core/multidimensional/slice.hpp>

#include <type_traits>
#include <sstream>

using namespace xmipp4::multidimensional;


TEST_CASE( "cross-constructing an slice should propagate values", "[slice]" )
{
    SECTION( "with integers" )
    {
        XMIPP4_CONST_CONSTEXPR auto a = make_slice(1, 3, 2);
        XMIPP4_CONST_CONSTEXPR auto b = slice<std::size_t, std::size_t, std::size_t>(a);

        REQUIRE( b.get_start() == 1 );
        REQUIRE( b.get_step() == 2 );
        REQUIRE( b.get_count() == 3 );
    }

    SECTION( "with integral constant" )
    {
        XMIPP4_CONST_CONSTEXPR auto a = make_slice(
            std::integral_constant<int, 1>(),
            std::integral_constant<int, 3>(),
            std::integral_constant<int, 2>()
        );
        XMIPP4_CONST_CONSTEXPR auto b = slice<std::size_t, std::size_t, std::size_t>(a);

        REQUIRE( b.get_start() == 1 );
        REQUIRE( b.get_step() == 2 );
        REQUIRE( b.get_count() == 3 );
    }

    SECTION( "with tags" )
    {
        XMIPP4_CONST_CONSTEXPR auto a = make_slice(std::size_t(8));
        XMIPP4_CONST_CONSTEXPR auto b = slice<int, int, int>(a);

        REQUIRE( b.get_start() == 0 );
        REQUIRE( b.get_step() == 1 );
        REQUIRE( b.get_count() == 8 );
    }

    SECTION( "integer promotion preserves end value")
    {
        const auto s0 = make_slice(1, end(), 2);
        const slice<int, int, std::uint8_t> s1(s0);
        const slice<int, int, std::int64_t> s2(s1);
        const slice<int, int, std::uint32_t> s3(s2);
        const slice<int, int, int> s4(s3);

        REQUIRE( s1.get_count() == end() );
        REQUIRE( s2.get_count() == end() );
        REQUIRE( s3.get_count() == end() );
        REQUIRE( s4.get_count() == end() );
    }
}

TEST_CASE( "make_slice with count argument should preserve its type and value", "[slice]" ) 
{
    SECTION( "with int" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(8);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const int>::value, 
            "count must be an int"
        );
        
        REQUIRE(count == 8);
    }

    SECTION( "with std::size_t" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(std::size_t(4));
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const std::size_t>::value, 
            "count must be a std::size_t"
        );
        
        REQUIRE(count == 4);
    }

    SECTION( "with end_tag" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(end());
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const end_tag>::value, 
            "count must be an end_tag"
        );
    }

    SECTION( "with integral constant" )
    {
        XMIPP4_CONST_CONSTEXPR auto five = std::integral_constant<int, 5>();
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(five);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const std::integral_constant<int, 5>>::value, 
            "count must be an std::integral_constant<int, 5>"
        );
    }
}

TEST_CASE( "make_slice with start and count arguments should preserve their types and values", "[slice]" ) 
{
    SECTION( "with int" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(1, 20);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const int>::value, 
            "Start must be an int"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const int>::value, 
            "count must be an int"
        );
        
        REQUIRE(start == 1);
        REQUIRE(count == 20);
    }

    SECTION( "with begin and end" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(begin(), end());
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const end_tag>::value, 
            "count must be an end_tag"
        );
    }

    SECTION( "with integral constant" )
    {
        XMIPP4_CONST_CONSTEXPR auto one = std::integral_constant<std::size_t, 1>();
        XMIPP4_CONST_CONSTEXPR auto five = std::integral_constant<int, 5>();
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(one, five);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const std::integral_constant<std::size_t, 1>>::value, 
            "Start must be a std::integral_constant<std::size_t, 1>"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const std::integral_constant<int, 5>>::value, 
            "count must be an std::integral_constant<int, 5>"
        );
    }
}

TEST_CASE( "make_slice with start stop and step arguments should preserve their types and values", "[slice]" ) 
{
    SECTION( "with int" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(1, 20, 2);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const int>::value, 
            "Start must be an int"
        );
        static_assert(
            std::is_same<decltype(step), const int>::value, 
            "Stride must be an int"
        );
        static_assert(
            std::is_same<decltype(count), const int>::value, 
            "count must be an int"
        );
        
        REQUIRE(start == 1);
        REQUIRE(step == 2);
        REQUIRE(count == 20);
    }

    SECTION( "with begin, adjacent and end" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(begin(), end(), adjacent());
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const end_tag>::value, 
            "count must be an end_tag"
        );
    }

    SECTION( "with integral constant" )
    {
        XMIPP4_CONST_CONSTEXPR auto one = std::integral_constant<std::size_t, 1>();
        XMIPP4_CONST_CONSTEXPR auto two = std::integral_constant<std::size_t, 2>();
        XMIPP4_CONST_CONSTEXPR auto five = std::integral_constant<int, 5>();
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(one, five, two);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const std::integral_constant<std::size_t, 1>>::value, 
            "Start must be a std::integral_constant<std::size_t, 1>"
        );
        static_assert(
            std::is_same<decltype(step), const std::integral_constant<std::size_t, 2>>::value, 
            "Stride must be an std::integral_constant<std::size_t, 2>"
        );
        static_assert(
            std::is_same<decltype(count), const std::integral_constant<int, 5>>::value, 
            "count must be an std::integral_constant<int, 5>"
        );
    }
}

TEST_CASE( "output slice to a std::ostream should produce the expected results", "[slice]" )
{
    std::stringstream stream;

    SECTION( "with int-s" )
    {
        XMIPP4_CONST_CONSTEXPR auto x = make_slice(1, 3, 2);
        stream << x;

        REQUIRE( stream.str() == "slice(1, 3, 2)" );
    }

    SECTION( "with tags" )
    {
        XMIPP4_CONST_CONSTEXPR auto x = make_slice(end());
        stream << x;

        REQUIRE( stream.str() == "slice(begin, end, adjacent)" );
    }
}
