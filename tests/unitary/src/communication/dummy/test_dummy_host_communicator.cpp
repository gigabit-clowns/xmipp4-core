// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <communication/dummy/dummy_host_communicator.hpp>

#include <xmipp4/core/communication/host_send_region.hpp>
#include <xmipp4/core/communication/host_send_receive_regions.hpp>
#include <xmipp4/core/communication/host_receive_region.hpp>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE("dummy_host_communicator should always have rank 0", "[dummy_host_communicator]")
{
	dummy_host_communicator comm;
	REQUIRE( comm.get_rank() == 0 );
}

TEST_CASE("dummy_host_communicator should always have a communicator size of 1", "[dummy_host_communicator]")
{
	dummy_host_communicator comm;
	REQUIRE( comm.get_size() == 1 );
}

TEST_CASE("dummy_host_communicator should be able to split regardless of the parameters", "[dummy_host_communicator]")
{
	dummy_host_communicator comm;
	int colour;
	int rank_priority;
	std::tie(colour, rank_priority) = GENERATE(
		table<int, int>({
			{0, 0},
			{0, 10},
			{1, 10},
			{-1, 0},
			{-1, -2},
		})
	);

	auto sub_comm = comm.split(colour, rank_priority);
	REQUIRE( sub_comm );
	REQUIRE( sub_comm->get_rank() == 0 );
	REQUIRE( sub_comm->get_size() == 1 );
}

TEST_CASE("dummy_host_communicator should throw when sending data to itself", "[dummy_host_communicator]")
{
	const auto tag = GENERATE(0, 10, -1);

	dummy_host_communicator comm;
	REQUIRE_THROWS_MATCHES(
		comm.create_send(
			host_send_region(),
			0,
			tag
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"Can not send data on the same rank that is receiving it"
		)
	);
}

TEST_CASE("dummy_host_communicator should throw sending with invalid destination rank", "[dummy_host_communicator]")
{
	const auto tag = GENERATE(0, 10, -1);
	const auto destination_rank = GENERATE(1, 2, 4, 10);

	dummy_host_communicator comm;
	REQUIRE_THROWS_MATCHES(
		comm.create_send(
			host_send_region(),
			destination_rank,
			tag
		),
		std::out_of_range,
		Catch::Matchers::Message(
			"destination_rank is out of bounds"
		)
	);
}

TEST_CASE("dummy_host_communicator should throw when attempting to receive data from itself", "[dummy_host_communicator]")
{
	const auto tag = GENERATE(0, 10, -1);

	dummy_host_communicator comm;
	REQUIRE_THROWS_MATCHES(
		comm.create_receive(
			host_receive_region(),
			0,
			tag
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"Can not receive data on the same rank that is sending it"
		)
	);
}

TEST_CASE("dummy_host_communicator should throw receiving with invalid source rank", "[dummy_host_communicator]")
{
	const auto tag = GENERATE(0, 10, -1);
	const auto source_rank = GENERATE(1, 2, 4, 10);

	dummy_host_communicator comm;
	REQUIRE_THROWS_MATCHES(
		comm.create_receive(
			host_receive_region(),
			source_rank,
			tag
		),
		std::out_of_range,
		Catch::Matchers::Message(
			"source_rank is out of bounds"
		)
	);
}
