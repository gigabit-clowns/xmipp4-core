// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <hardware/host_memory/host_buffer_sentinel.hpp>

#include "../mock/mock_device_queue.hpp"

#include <sstream>


using namespace xmipp4::hardware;

TEST_CASE( "host_buffer_sentinel should allocate enough space", "[host_buffer_sentinel]" )
{
    const std::size_t size = 1024;
    const std::size_t alignment = 64;

    host_buffer_sentinel sentinel(size, alignment);

    REQUIRE( sentinel.get_data() );
    REQUIRE( reinterpret_cast<std::uintptr_t>(sentinel.get_data()) % alignment == 0 );
    std::memset(sentinel.get_data(), 0, size); // Should not segfault
}

TEST_CASE( "host_buffer_sentinel should throw when recording queues", "[host_buffer_sentinel]" )
{
    const std::size_t size = 1024;
    const std::size_t alignment = 64;
    host_buffer_sentinel sentinel(size, alignment);

    mock_device_queue queue;

    REQUIRE_THROWS_MATCHES(
        sentinel.record_queue(queue, false),
		std::invalid_argument,
		Catch::Matchers::Message(
            "host_buffer_sentinel does not support queue execution"
        )
	);

    REQUIRE_THROWS_MATCHES(
        sentinel.record_queue(queue, true),
		std::invalid_argument,
		Catch::Matchers::Message(
            "host_buffer_sentinel does not support queue execution"
        )
	);
}
