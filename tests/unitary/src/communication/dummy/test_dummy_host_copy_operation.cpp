// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <communication/dummy/dummy_host_copy_operation.hpp>

#include <array>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "dummy_host_copy_operation should copy data between source and destination regions with execute", "[dummy_host_copy_operation]" )
{
	std::array<std::size_t, 1024> source;
	std::array<std::size_t, 1024> destination = {};
	for (std::size_t i = 0; i < source.size(); ++i)
	{
		source[i] = i;
	}

	dummy_host_copy_operation operation(
		source.data(),
		destination.data(),
		source.size() * sizeof(std::size_t)
	);

	operation.execute();
	for (std::size_t i = 0; i < destination.size(); ++i)
	{
		REQUIRE( destination[i] == i );
	}

	for (std::size_t i = 0; i < source.size(); ++i)
	{
		source[i] = 0xDEADBEEF;
	}

	operation.execute();
	for (std::size_t i = 0; i < destination.size(); ++i)
	{
		REQUIRE( destination[i] == 0xDEADBEEF );
	}
}
