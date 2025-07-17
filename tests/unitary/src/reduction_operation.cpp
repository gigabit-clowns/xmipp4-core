// SPDX-License-Identifier: GPL-3.0-only

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

