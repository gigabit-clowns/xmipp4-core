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
 * @file test_pimpl.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for memory/pimpl.hpp
 * @date 2024-03-10
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/memory/pimpl.hpp>

using namespace xmipp4;

struct test_implementation
{
    int first;
    double second;
    std::string third;

    test_implementation(int first_arg, double second_arg, std::string third_arg)
        : first(first_arg)
        , second(second_arg)
        , third(std::move(third_arg))
    {
    }
};

using test_pimpl = memory::pimpl<test_implementation>;


TEST_CASE( "construct a pointer to implementation", "[pimpl]" ) 
{
    test_pimpl x(1, 2.3, "Lorem ipsum");
    REQUIRE( static_cast<bool>(x) == true );
    REQUIRE( x->first == 1 );
    REQUIRE( x->second == 2.3 );
    REQUIRE( x->third == "Lorem ipsum" );

    x.reset();
    REQUIRE( static_cast<bool>(x) == false );

    x.emplace(2, 6.4, "Hello world");
    REQUIRE( static_cast<bool>(x) == true );
    REQUIRE( x->first == 2 );
    REQUIRE( x->second == 6.4 );
    REQUIRE( x->third == "Hello world" );
}

TEST_CASE( "construct a pointer to implementation from other", "[pimpl]" ) 
{
    test_pimpl x(1, 2.3, "Lorem ipsum");

    SECTION("copy construct")
    {
        test_pimpl y(x);
        REQUIRE( static_cast<bool>(x) == true );
        REQUIRE( x->first == 1 );
        REQUIRE( x->second == 2.3 );
        REQUIRE( x->third == "Lorem ipsum" );

        REQUIRE( static_cast<bool>(y) == true );
        REQUIRE( y->first == 1 );
        REQUIRE( y->second == 2.3 );
        REQUIRE( y->third == "Lorem ipsum" );
    }

    SECTION("move construct")
    {
        test_pimpl y(std::move(x));
        REQUIRE( static_cast<bool>(x) == false );

        REQUIRE( static_cast<bool>(y) == true );
        REQUIRE( y->first == 1 );
        REQUIRE( y->second == 2.3 );
        REQUIRE( y->third == "Lorem ipsum" );
    }

    SECTION("copy assign")
    {
        test_pimpl y(memory::defer_construct);
        y = x;
        REQUIRE( static_cast<bool>(x) == true );
        REQUIRE( x->first == 1 );
        REQUIRE( x->second == 2.3 );
        REQUIRE( x->third == "Lorem ipsum" );

        REQUIRE( static_cast<bool>(y) == true );
        REQUIRE( y->first == 1 );
        REQUIRE( y->second == 2.3 );
        REQUIRE( y->third == "Lorem ipsum" );
    }

    SECTION("move assign")
    {
        test_pimpl y(memory::defer_construct);
        y = std::move(x);
        REQUIRE( static_cast<bool>(x) == false );


        REQUIRE( static_cast<bool>(y) == true );
        REQUIRE( y->first == 1 );
        REQUIRE( y->second == 2.3 );
        REQUIRE( y->third == "Lorem ipsum" );
    }

    SECTION("swap")
    {
        test_pimpl y(3, 6.7, "Hello world");
        swap(x, y);

        REQUIRE( static_cast<bool>(x) == true );
        REQUIRE( x->first == 3 );
        REQUIRE( x->second == 6.7 );
        REQUIRE( x->third == "Hello world" );

        REQUIRE( static_cast<bool>(y) == true );
        REQUIRE( y->first == 1 );
        REQUIRE( y->second == 2.3 );
        REQUIRE( y->third == "Lorem ipsum" );
    }
}

TEST_CASE( "construct a deferred pointer to implementation", "[pimpl]" ) 
{
    test_pimpl x(memory::defer_construct);
    REQUIRE( static_cast<bool>(x) == false );

    x.reset();
    REQUIRE( static_cast<bool>(x) == false );
}
