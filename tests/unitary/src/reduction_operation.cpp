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
 * @file test_reduction_operation.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for reduction_operation.hpp
 * @date 2025-03-27
 */


#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/reduction_operation.hpp>

using namespace xmipp4;


TEST_CASE( "to_string with reduction_operation should produce correct results", "[reduction_operation]" ) 
{
    REQUIRE( std::string(to_string(reduction_operation::sum)) == "sum" ); 
    REQUIRE( std::string(to_string(reduction_operation::product)) == "product" ); 
    REQUIRE( std::string(to_string(reduction_operation::min)) == "min" ); 
    REQUIRE( std::string(to_string(reduction_operation::max)) == "max" ); 
}

TEST_CASE( "from_string with reduction_operation should produce correct results", "[reduction_operation]" ) 
{
    reduction_operation op;

    REQUIRE( from_string("sum", op) );
    REQUIRE( op == reduction_operation::sum );  
    REQUIRE( from_string("product", op) );
    REQUIRE( op == reduction_operation::product );  
    REQUIRE( from_string("max", op) );
    REQUIRE( op == reduction_operation::max );
    REQUIRE( from_string("min", op) );
    REQUIRE( op == reduction_operation::min );
    
    REQUIRE( !from_string("invalid", op) );
}

