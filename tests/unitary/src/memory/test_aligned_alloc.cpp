// SPDX-License-Identifier: GPL-3.0-only


#include <xmipp4/core/memory/aligned_alloc.hpp>

#include <xmipp4/core/memory/align.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace xmipp4;

TEST_CASE( "aligned_alloc should produce aligned allocations", "[aligned_alloc]" ) 
{
	const std::size_t repetitions = 8192;
	const std::size_t alignment = 512; // Large enough power of two.
	const std::size_t size = 10*alignment;

	for (std::size_t i = 0; i < repetitions; ++i)
	{
		auto *data = static_cast<char*>(memory::aligned_alloc(size, alignment));
		REQUIRE( data != nullptr ); // Assuming no OOM situation. 
		REQUIRE( memory::get_alignment(data) >= alignment );

		// Test that it can be written (no segfault)
		for(std::size_t j = 0; j < size; ++j)
		{
			data[j] = 0xAA;
		}

		memory::aligned_free(data);
	}

}
