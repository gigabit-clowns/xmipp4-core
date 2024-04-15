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
 * @file test_axis_3d.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for axis_3d.hpp
 * @date 2023-09-25
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/axis_3d.hpp>

using namespace xmipp4;


TEST_CASE( "operator-", "[axis_3d]" ) 
{
    REQUIRE( -axis_3d::zero == axis_3d::zero );
    REQUIRE( -axis_3d::x == axis_3d::negative_x );
    REQUIRE( -axis_3d::y == axis_3d::negative_y );
    REQUIRE( -axis_3d::z == axis_3d::negative_z );
    REQUIRE( -axis_3d::negative_x == axis_3d::x );
    REQUIRE( -axis_3d::negative_y == axis_3d::y );
    REQUIRE( -axis_3d::negative_z == axis_3d::z );
}

TEST_CASE( "is_positive", "[axis_3d]" ) 
{
    REQUIRE( is_positive(axis_3d::zero) == false );
    REQUIRE( is_positive(axis_3d::x) == true );
    REQUIRE( is_positive(axis_3d::y) == true );
    REQUIRE( is_positive(axis_3d::z) == true );
    REQUIRE( is_positive(axis_3d::negative_x) == false );
    REQUIRE( is_positive(axis_3d::negative_y) == false );
    REQUIRE( is_positive(axis_3d::negative_z) == false );
}

TEST_CASE( "is_negative", "[axis_3d]" ) 
{
    REQUIRE( is_negative(axis_3d::zero) == false );
    REQUIRE( is_negative(axis_3d::x) == false );
    REQUIRE( is_negative(axis_3d::y) == false );
    REQUIRE( is_negative(axis_3d::z) == false );
    REQUIRE( is_negative(axis_3d::negative_x) == true );
    REQUIRE( is_negative(axis_3d::negative_y) == true );
    REQUIRE( is_negative(axis_3d::negative_z) == true );
}

TEST_CASE( "next_axis", "[axis_3d]" ) 
{
    REQUIRE( next_axis(axis_3d::zero) == axis_3d::zero );
    REQUIRE( next_axis(axis_3d::x) == axis_3d::y );
    REQUIRE( next_axis(axis_3d::y) == axis_3d::z );
    REQUIRE( next_axis(axis_3d::z) == axis_3d::x );
    REQUIRE( next_axis(axis_3d::negative_x) == axis_3d::negative_z );
    REQUIRE( next_axis(axis_3d::negative_y) == axis_3d::negative_x );
    REQUIRE( next_axis(axis_3d::negative_z) == axis_3d::negative_y );
}

TEST_CASE( "dot", "[axis_3d]" ) 
{
    SECTION( "One of the elements is zero" )
    {
        REQUIRE( dot(axis_3d::zero, axis_3d::zero) == 0 );
        REQUIRE( dot(axis_3d::x, axis_3d::zero) == 0 );
        REQUIRE( dot(axis_3d::zero, axis_3d::y) == 0 );
        REQUIRE( dot(axis_3d::negative_z, axis_3d::zero) == 0 );
    }
    
    SECTION( "Same axis" )
    {
        REQUIRE( dot(axis_3d::x, axis_3d::x) == 1 );
        REQUIRE( dot(axis_3d::y, axis_3d::y) == 1 );
        REQUIRE( dot(axis_3d::z, axis_3d::z) == 1 );
        REQUIRE( dot(axis_3d::negative_x, axis_3d::negative_x) == 1 );
        REQUIRE( dot(axis_3d::negative_y, axis_3d::negative_y) == 1 );
        REQUIRE( dot(axis_3d::negative_z, axis_3d::negative_z) == 1 );
    }
    
    SECTION( "Opposite axis" )
    {
        REQUIRE( dot(axis_3d::x, axis_3d::negative_x) == -1 );
        REQUIRE( dot(axis_3d::y, axis_3d::negative_y) == -1 );
        REQUIRE( dot(axis_3d::z, axis_3d::negative_z) == -1 );
        REQUIRE( dot(axis_3d::negative_x, axis_3d::x) == -1 );
        REQUIRE( dot(axis_3d::negative_y, axis_3d::y) == -1 );
        REQUIRE( dot(axis_3d::negative_z, axis_3d::z) == -1 );
    }
    
    SECTION( "Different axis" )
    {
        REQUIRE( dot(axis_3d::x, axis_3d::y) == 0 );
        REQUIRE( dot(axis_3d::y, axis_3d::negative_x) == 0 );
        REQUIRE( dot(axis_3d::negative_z, axis_3d::negative_y) == 0 );
    }
}

TEST_CASE( "cross", "[axis_3d]" ) 
{
    SECTION( "One of the elements is zero" )
    {
        REQUIRE( cross(axis_3d::zero, axis_3d::zero) == axis_3d::zero );
        REQUIRE( cross(axis_3d::x, axis_3d::zero) == axis_3d::zero );
        REQUIRE( cross(axis_3d::zero, axis_3d::y) == axis_3d::zero );
        REQUIRE( cross(axis_3d::negative_z, axis_3d::zero) == axis_3d::zero );
    }
    
    SECTION( "Same axis" )
    {
        REQUIRE( cross(axis_3d::x, axis_3d::x) == axis_3d::zero );
        REQUIRE( cross(axis_3d::y, axis_3d::y) == axis_3d::zero );
        REQUIRE( cross(axis_3d::z, axis_3d::z) == axis_3d::zero );
        REQUIRE( cross(axis_3d::negative_x, axis_3d::negative_x) == axis_3d::zero );
        REQUIRE( cross(axis_3d::negative_y, axis_3d::negative_y) == axis_3d::zero );
        REQUIRE( cross(axis_3d::negative_z, axis_3d::negative_z) == axis_3d::zero );
    }
    
    SECTION( "Opposite axis" )
    {
        REQUIRE( cross(axis_3d::x, axis_3d::negative_x) == axis_3d::zero );
        REQUIRE( cross(axis_3d::y, axis_3d::negative_y) == axis_3d::zero );
        REQUIRE( cross(axis_3d::z, axis_3d::negative_z) == axis_3d::zero );
        REQUIRE( cross(axis_3d::negative_x, axis_3d::x) == axis_3d::zero );
        REQUIRE( cross(axis_3d::negative_y, axis_3d::y) == axis_3d::zero );
        REQUIRE( cross(axis_3d::negative_z, axis_3d::z) == axis_3d::zero );
    }
    
    SECTION( "Different axis" )
    {
        SECTION( "Standard basis" )
        {
            REQUIRE( cross(axis_3d::x, axis_3d::y) == axis_3d::z );
            REQUIRE( cross(axis_3d::y, axis_3d::z) == axis_3d::x );
            REQUIRE( cross(axis_3d::z, axis_3d::x) == axis_3d::y );
        }
        SECTION( "Reversed standard basis" )
        {
            REQUIRE( cross(axis_3d::y, axis_3d::x) == axis_3d::negative_z );
            REQUIRE( cross(axis_3d::z, axis_3d::y) == axis_3d::negative_x );
            REQUIRE( cross(axis_3d::x, axis_3d::z) == axis_3d::negative_y );
        }
        SECTION( "Random combinations" )
        {
            REQUIRE( cross(axis_3d::negative_y, axis_3d::x) == axis_3d::z );
            REQUIRE( cross(axis_3d::z, axis_3d::negative_y) == axis_3d::x );
            REQUIRE( cross(axis_3d::negative_z, axis_3d::negative_y) == axis_3d::negative_x );
            REQUIRE( cross(axis_3d::negative_x, axis_3d::z) == axis_3d::y );
            REQUIRE( cross(axis_3d::negative_x, axis_3d::negative_y) == axis_3d::z );
        }
    }
}