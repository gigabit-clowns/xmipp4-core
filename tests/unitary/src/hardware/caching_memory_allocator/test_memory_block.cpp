// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <hardware/caching_memory_allocator/memory_block.hpp>

#include <xmipp4/core/hardware/buffer_sentinel.hpp>

#include "../mock/mock_device_queue.hpp"
#include "../mock/mock_memory_heap.hpp"

#include <cstddef>

using namespace xmipp4::hardware;

TEST_CASE( "constructing a memory_block should store the arguments", "[caching_memory_allocator]" )
{
	mock_memory_heap heap;
	const std::size_t offset = 1234;
	const std::size_t size = 9876;
	mock_device_queue queue;

	memory_block block(&queue, size, &heap, offset);

	REQUIRE( block.get_queue() == &queue );
	REQUIRE( block.get_size() == size );
	REQUIRE( block.get_heap() == &heap );
	REQUIRE( block.get_offset() == offset );
}

TEST_CASE( "setting the size in a memory heap should store its value", "[caching_memory_allocator]" )
{
	mock_memory_heap heap;
	const std::size_t offset = 1234;
	const std::size_t size = 9876;
	mock_device_queue queue;

	memory_block block(&queue, size, &heap, offset);

	const std::size_t new_size = 6543;
	block.set_size(new_size);
	REQUIRE( block.get_size() == new_size );
}

TEST_CASE( "setting the offset in a memory heap should store its value", "[caching_memory_allocator]" )
{
	mock_memory_heap heap;
	const std::size_t offset = 1234;
	const std::size_t size = 9876;
	mock_device_queue queue;

	memory_block block(&queue, size, &heap, offset);

	const std::size_t new_offset = 6543;
	block.set_offset(new_offset);
	REQUIRE( block.get_offset() == new_offset );
}
