// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/reduction_operation.hpp>

using namespace xmipp4;


TEST_CASE( "to_string with reduction_operation should produce correct results", "[reduction_operation]" ) 
{
    reduction_operation reduction;
    std::string expected_str;
    std::tie(reduction, expected_str) = GENERATE(
        table<reduction_operation, std::string>({
            {reduction_operation::sum, "sum"},
            {reduction_operation::product, "product"},
            {reduction_operation::min, "min"},
            {reduction_operation::max, "max"}
        })
    );

    REQUIRE( std::string(to_string(reduction)) == expected_str );
}

TEST_CASE( "from_string with reduction_operation should produce correct results", "[reduction_operation]" ) 
{
    std::string input_str;
    bool expected_valid;
    reduction_operation expected_reduction;
    std::tie(input_str, expected_valid, expected_reduction) = GENERATE(
        table<std::string, bool, reduction_operation>({
            {"sum",     true, reduction_operation::sum},
            {"product", true, reduction_operation::product},
            {"max",     true, reduction_operation::max},
            {"min",     true, reduction_operation::min},
            {"invalid", false, reduction_operation::sum} // op value doesn't matter if invalid
        })
    );

    reduction_operation reduction;
    bool valid = from_string(input_str, reduction);
    REQUIRE(valid == expected_valid);
    if (expected_valid)
    {
        REQUIRE( reduction == expected_reduction );
    }
}
