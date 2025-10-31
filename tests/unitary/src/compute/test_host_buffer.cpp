// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <compute/host_buffer.hpp>

#include <xmipp4/core/compute/memory_resource.hpp>

#include "mock/mock_device_queue.hpp"

#include <sstream>


using namespace xmipp4::compute;

TEST_CASE( "default constructing a host_buffer should create an empty buffer", "[host_buffer]" )
{
    host_buffer buffer;

    REQUIRE( buffer.get_size() == 0 );
    REQUIRE( buffer.get_host_ptr() == nullptr );
}

TEST_CASE( "constructing a host_buffer should allocate enough space", "[host_buffer]" )
{
    const std::size_t size = 1024;
    const std::size_t alignment = 64;

    host_buffer buffer(size, alignment);

    std::memset(buffer.get_host_ptr(), 0, buffer.get_size()); // Should not segfault

    REQUIRE( buffer.get_size() >= size );
    REQUIRE( reinterpret_cast<std::uintptr_t>(buffer.get_host_ptr()) % alignment == 0 );
}

TEST_CASE( "move constructing a host_buffer should move its resources", "[host_buffer]" )
{
    const std::size_t size = 1024;
    const std::size_t alignment = 64;

    host_buffer buffer1(size, alignment);
    const auto original_size = buffer1.get_size();
    const auto original_ptr = buffer1.get_host_ptr();

    REQUIRE( original_size >= size );
    REQUIRE( original_ptr != nullptr );

    auto buffer2 = std::move(buffer1);
    REQUIRE( buffer2.get_size() == original_size );
    REQUIRE( buffer2.get_host_ptr() == original_ptr );
    REQUIRE( buffer1.get_size() == 0 );
    REQUIRE( buffer1.get_host_ptr() == nullptr );
}

TEST_CASE( "move assigning a host_buffer should move its resources", "[host_buffer]" )
{
    const std::size_t size = 1024;
    const std::size_t alignment = 64;

    host_buffer buffer1(size, alignment);
    const auto original_size = buffer1.get_size();
    const auto original_ptr = buffer1.get_host_ptr();

    REQUIRE( original_size >= size );
    REQUIRE( original_ptr != nullptr );

    host_buffer buffer2(size + 2048, alignment);
    buffer2 = std::move(buffer1);
    REQUIRE( buffer2.get_size() == original_size );
    REQUIRE( buffer2.get_host_ptr() == original_ptr );
    REQUIRE( buffer1.get_size() == 0 );
    REQUIRE( buffer1.get_host_ptr() == nullptr );
}

TEST_CASE( "resetting a host_buffer should free its resources", "[host_buffer]" )
{
    const std::size_t size = 1024;
    const std::size_t alignment = 64;

    host_buffer buffer(size, alignment);
    REQUIRE( buffer.get_size() >= size );
    REQUIRE( buffer.get_host_ptr() != nullptr );

    buffer.reset();
    REQUIRE( buffer.get_size() == 0 );
    REQUIRE( buffer.get_host_ptr() == nullptr );
}

TEST_CASE( "swapping two host_buffers should exchange their resources", "[host_buffer]" )
{
    const std::size_t size1 = 1024;
    const std::size_t size2 = 2048;
    const std::size_t alignment = 64;

    host_buffer buffer1(size1, alignment);
    host_buffer buffer2(size2, alignment);

    const auto original_size1 = buffer1.get_size();
    const auto original_ptr1 = buffer1.get_host_ptr();
    const auto original_size2 = buffer2.get_size();
    const auto original_ptr2 = buffer2.get_host_ptr();

    buffer1.swap(buffer2);

    REQUIRE( buffer1.get_size() == original_size2 );
    REQUIRE( buffer1.get_host_ptr() == original_ptr2 );
    REQUIRE( buffer2.get_size() == original_size1 );
    REQUIRE( buffer2.get_host_ptr() == original_ptr1 );
}

TEST_CASE( "recording a device_queue on a host_buffer should synchronize the queue", "[host_buffer]" )
{
    const std::size_t size = 1024;
    const std::size_t alignment = 64;

    host_buffer buffer(size, alignment);
    mock_device_queue queue;

    REQUIRE_CALL(queue, wait_until_completed());
    buffer.record_queue(queue, false);

    REQUIRE_CALL(queue, wait_until_completed());
    buffer.record_queue(queue, true);
}

TEST_CASE( "host_buffer should return the host memory resource in get_memory_resource", "[host_buffer]" )
{
    const std::size_t size = 1024;
    const std::size_t alignment = 64;

    host_buffer buffer(size, alignment);
    auto& resource1 = buffer.get_memory_resource();
    auto& resource2 = get_host_memory_resource();

    REQUIRE( &resource1 == &resource2 );
}
