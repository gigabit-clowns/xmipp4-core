// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <hardware/host_memory/host_memory_allocator.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

#include "../mock/mock_device_queue.hpp"

#include <sstream>


using namespace xmipp4::hardware;

TEST_CASE( "host_memory_allocator should return a host accessible buffer", "[host_memory_allocator]" )
{
    host_memory_allocator allocator;

    const std::size_t size = 1024;
    const std::size_t alignment = 64;
    auto buffer = allocator.allocate(size, alignment, nullptr);

    REQUIRE( buffer != nullptr );
    REQUIRE( reinterpret_cast<std::uintptr_t>(buffer->get_host_ptr()) % alignment == 0 );
    REQUIRE( buffer->get_size() >= size );
}

TEST_CASE( "host_memory_allocator should wait the queue to complete", "[host_memory_allocator]" )
{
    host_memory_allocator allocator;

    mock_device_queue queue;

    REQUIRE_CALL(queue, wait_until_completed());

    const std::size_t size = 1024;
    const std::size_t alignment = 64;
    allocator.allocate(size, alignment, &queue);
}

TEST_CASE( "host_memory_resource should return the host memory resource in get_memory_resource", "[host_memory_allocator]" )
{
    host_memory_allocator allocator;

    auto& resource1 = allocator.get_memory_resource();
    auto& resource2 = get_host_memory_resource();

    REQUIRE( &resource1 == &resource2 );
}

TEST_CASE( "host_buffer_allocator should allocate enough space", "[host_memory_allocator]" )
{
    const std::size_t size = 1024;
    const std::size_t alignment = 64;

    host_memory_allocator allocator;
    auto buffer = allocator.allocate(size, alignment, nullptr);

    REQUIRE( buffer->get_size() >= size );
    REQUIRE( reinterpret_cast<std::uintptr_t>(buffer->get_host_ptr()) % alignment == 0 );

    std::memset(buffer->get_host_ptr(), 0, buffer->get_size()); // Should not segfault
}
