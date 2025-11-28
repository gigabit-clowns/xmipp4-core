// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/communication/reduction_operation.hpp>

using namespace xmipp4;
using namespace xmipp4::communication;

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
