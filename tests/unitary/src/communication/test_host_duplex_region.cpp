// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/communication/host_duplex_region.hpp>

#include <array>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "default constructed host_duplex_region should be empty ", "[host_duplex_region]" ) 
{
	const host_duplex_region region;
	REQUIRE( region.get_send_data() == nullptr );
	REQUIRE( region.get_receive_data() == nullptr );
	REQUIRE( region.get_data_type() == numerical_type::unknown );
	REQUIRE( region.get_count() == 0UL );
}

TEST_CASE( "constructing a host_duplex_region from raw pointers should store its attributes", "[host_duplex_region]" ) 
{
	const std::array<char, 1024> send_data = {}; 
	std::array<char, 1024> receive_data = {}; 

	const auto data_type = GENERATE(
		numerical_type::int16,
		numerical_type::float64
	);
	const auto count = GENERATE(10, 20);

	const host_duplex_region region(
		send_data.data(),
		receive_data.data(),
		data_type,
		count
	);

	REQUIRE( region.get_send_data() == send_data.data() );
	REQUIRE( region.get_receive_data() == receive_data.data() );
	REQUIRE( region.get_data_type() == data_type );
	REQUIRE( region.get_count() == count );
}

TEST_CASE( "constructing a host_duplex_region from statically typed pointers store its attributes", "[host_duplex_region]" ) 
{
	const std::array<float32_t, 1024> send_data = {}; 
	std::array<float32_t, 1024> receive_data = {}; 
	const host_duplex_region region(
		send_data.data(),
		receive_data.data(),
		send_data.size()
	);

	REQUIRE( region.get_send_data() == send_data.data() );
	REQUIRE( region.get_receive_data() == receive_data.data() );
	REQUIRE( region.get_data_type() == numerical_type::float32 );
	REQUIRE( region.get_count() == send_data.size() );
}

TEST_CASE( "constructing a host_duplex_region from a single pointer should create an aliasing region", "[host_duplex_region]" ) 
{
	std::array<char, 1024> send_recv_data = {}; 

	const auto data_type = GENERATE(
		numerical_type::int16,
		numerical_type::float64
	);
	const auto count = GENERATE(10, 20);

	const host_duplex_region region(
		send_recv_data.data(),
		data_type,
		count
	);

	REQUIRE( region.get_send_data() == send_recv_data.data() );
	REQUIRE( region.get_receive_data() == send_recv_data.data() );
	REQUIRE( region.get_data_type() == data_type );
	REQUIRE( region.get_count() == count );
}

TEST_CASE( "constructing a host_duplex_region from statically typed pointer should create an aliasing region ", "[host_duplex_region]" ) 
{
	std::array<float32_t, 1024> send_recv_data = {}; 

	const host_duplex_region region(
		send_recv_data.data(),
		send_recv_data.size()
	);

	REQUIRE( region.get_send_data() == send_recv_data.data() );
	REQUIRE( region.get_receive_data() == send_recv_data.data() );
	REQUIRE( region.get_data_type() == numerical_type::float32 );
	REQUIRE( region.get_count() == send_recv_data.size() );
}
