// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/communication/host_send_region.hpp>

#include <array>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "default constructed host_send_region should be empty ", "[host_send_region]" ) 
{
	const host_send_region region;
	REQUIRE( region.get_data() == nullptr );
	REQUIRE( region.get_data_type() == numerical_type::unknown );
	REQUIRE( region.get_count() == 0UL );
}

TEST_CASE( "constructing a host_send_region from a raw pointer should store its attributes", "[host_send_region]" ) 
{
	const std::array<char, 1024> data = {}; 

	const auto data_type = GENERATE(
		numerical_type::int16,
		numerical_type::float64
	);
	const auto count = GENERATE(10, 20);

	const host_send_region region(
		data.data(),
		data_type,
		count
	);

	REQUIRE( region.get_data() == data.data() );
	REQUIRE( region.get_data_type() == data_type );
	REQUIRE( region.get_count() == count );
}

TEST_CASE( "constructing a host_send_region from a statically typed pointer store its attributes", "[host_send_region]" ) 
{
	const std::array<float32_t, 1024> data = {}; 
	const host_send_region region(
		data.data(),
		data.size()
	);

	REQUIRE( region.get_data() == data.data() );
	REQUIRE( region.get_data_type() == numerical_type::float32 );
	REQUIRE( region.get_count() == data.size() );
}
