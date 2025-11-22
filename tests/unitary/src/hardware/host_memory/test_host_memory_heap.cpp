// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <hardware/host_memory/host_memory_heap.hpp>

#include <xmipp4/core/hardware/buffer.hpp>

#include "../mock/mock_device_queue.hpp"
#include "../mock/mock_buffer_sentinel.hpp"

#include <stdexcept>

using namespace xmipp4::hardware;

TEST_CASE( "host_memory_heap should create host accessible buffers", "[host_memory_heap]" )
{
	const std::size_t heap_size = 1024;
	const std::size_t heap_alignment = 64;
	const std::size_t buffer_size = 512;
	const std::size_t buffer_offset = 64;

	host_memory_heap heap(heap_size, heap_alignment);
	auto buffer = heap.create_buffer(buffer_offset, buffer_size, nullptr);

	REQUIRE( buffer );
	REQUIRE( buffer->get_host_ptr() );
	REQUIRE( buffer->get_size() == buffer_size );

	std::memset(buffer->get_host_ptr(), 0, buffer_size); // Should not segfault
}

TEST_CASE( "host_memory_heap should throw with out of bounds allocation", "[host_memory_heap]" )
{
	const std::size_t heap_size = 1024;
	const std::size_t heap_alignment = 64;
	const std::size_t buffer_size = 1023;
	const std::size_t buffer_offset = 2;

	host_memory_heap heap(heap_size, heap_alignment);

	REQUIRE_THROWS_MATCHES(
		heap.create_buffer(buffer_offset, buffer_size, nullptr),
		std::out_of_range,
		Catch::Matchers::Message("Requested allocation exceeds heap size")
	);
}

TEST_CASE( "host_memory_heap should pass the buffer_sentinel to the buffer", "[host_memory_heap]" )
{
	const std::size_t heap_size = 1024;
	const std::size_t heap_alignment = 64;
	const std::size_t buffer_size = 512;
	const std::size_t buffer_offset = 0;

	host_memory_heap heap(heap_size, heap_alignment);
	auto sentinel = std::make_unique<mock_buffer_sentinel>();

	mock_device_queue queue;
	REQUIRE_CALL((*sentinel), record_queue(ANY(device_queue&), true))
		.LR_WITH(&_1 == &queue);

	auto buffer = heap.create_buffer(buffer_offset, buffer_size, std::move(sentinel) );
	buffer->record_queue(queue, true);
}
