// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/unified_memory_allocator.hpp>

#include "mock/mock_unified_memory_allocator.hpp"
#include "mock/mock_device_queue.hpp"
#include "mock/mock_unified_buffer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

using namespace xmipp4;
using namespace xmipp4::compute;

TEST_CASE( "create_device_buffer should call the queued version of create_unified_buffer in unified_memory_allocator", "[unified_memory_allocator]" )
{
    mock_unified_memory_allocator mock;
    mock_device_queue queue;

    const std::size_t size = 12345678;
    const std::size_t alignment = 946372434;
    const auto return_value = std::make_shared<mock_unified_buffer>();

    REQUIRE_CALL(mock, create_unified_buffer(size, alignment, trompeloeil::_))
        .LR_WITH(&_3 == &queue)
        .RETURN(return_value);
    
    const auto result = mock.create_device_buffer(size, alignment, queue);

    REQUIRE( result == return_value );
}

TEST_CASE( "create_host_buffer with queue argument should call the queued version of create_unified_buffer in unified_memory_allocator", "[unified_memory_allocator]" )
{
    mock_unified_memory_allocator mock;
    mock_device_queue queue;

    const std::size_t size = 12345678;
    const std::size_t alignment = 946372434;
    const auto return_value = std::make_shared<mock_unified_buffer>();

    REQUIRE_CALL(mock, create_unified_buffer(size, alignment, trompeloeil::_))
        .LR_WITH(&_3 == &queue)
        .RETURN(return_value);
    
    const auto result = mock.create_host_buffer(size, alignment, queue);

    REQUIRE( result == return_value );
}

TEST_CASE( "create_host_buffer without queue argument should call the queueless version of create_unified_buffer in unified_memory_allocator", "[unified_memory_allocator]" )
{
    mock_unified_memory_allocator mock;

    const std::size_t size = 12345678;
    const std::size_t alignment = 946372434;
    const auto return_value = std::make_shared<mock_unified_buffer>();

    REQUIRE_CALL(mock, create_unified_buffer(size, alignment))
        .RETURN(return_value);
    
    const auto result = mock.create_host_buffer(size, alignment);

    REQUIRE( result == return_value );
}
