// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file test_access_flags.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for access_flags.hpp
 * @date 2025-03-27
 */


#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/access_flags.hpp>

using namespace xmipp4;


TEST_CASE( "to_string with access_flag_bits should produce correct results", "[access_flags]" ) 
{
    REQUIRE( std::string(to_string(access_flag_bits::read)) == "read" ); 
    REQUIRE( std::string(to_string(access_flag_bits::write)) == "write" ); 
}
