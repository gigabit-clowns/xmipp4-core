// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <hardware/caching_memory_allocator/memory_block.hpp>

#include "../mock/mock_device_queue.hpp"
#include "../mock/mock_memory_heap.hpp"

#include <cstddef>


using namespace xmipp4::hardware;

TEST_CASE( "Constructing a memory_block should store the arguments", "[caching_memory_allocator]" )
{
    auto heap = std::make_shared<mock_memory_heap>();
    const std::size_t offset = 1234;
    const std::size_t size = 9876;
    mock_device_queue queue;

    
    memory_block block(heap, offset, size, &queue);

    REQUIRE( block.get_heap() == heap.get() );
    REQUIRE( block.share_heap() == heap );
    REQUIRE( block.get_offset() == offset );
    REQUIRE( block.get_size() == size );
    REQUIRE( block.get_queue() == &queue );
}
