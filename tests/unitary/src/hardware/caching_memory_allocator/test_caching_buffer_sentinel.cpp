// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <hardware/caching_memory_allocator/caching_buffer_sentinel.hpp>

#include <hardware/caching_memory_allocator/caching_memory_allocator.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

#include "../mock/mock_device_queue.hpp"
#include "../mock/mock_memory_resource.hpp"
#include "../mock/mock_memory_heap.hpp"

#include <algorithm>
#include <cstddef>

using namespace xmipp4::hardware;

TEST_CASE( "adding queues to a caching_buffer_sentinel should store them" )
{
    const std::size_t max_alignment = 256;
    const std::size_t request_size_step = 1<<20;
    mock_memory_resource resource;

    REQUIRE_CALL(resource, get_target_device())
        .RETURN(nullptr);
    caching_memory_allocator allocator(
        resource, 
        max_alignment, 
        request_size_step
    );

    mock_device_queue queue1;
    mock_device_queue queue2;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(request_size_step);
    REQUIRE_CALL(*heap, create_buffer(0, 256, ANY(std::unique_ptr<buffer_sentinel>)))
        .LR_RETURN(std::make_shared<buffer>(nullptr, 256, resource, std::move(_3)));
    REQUIRE_CALL(resource, create_memory_heap(request_size_step, max_alignment))
        .RETURN(heap);

    auto buffer = allocator.allocate(20, 32, nullptr);
    auto* sentinel = dynamic_cast<caching_buffer_sentinel*>(buffer->get_sentinel());
    sentinel->record_queue(queue2, false);
    sentinel->record_queue(queue1, false);

    const auto queues = sentinel->get_queues();
    REQUIRE( queues.size() == 2 );
    REQUIRE( queues[0] == std::min(&queue1, &queue2) );
    REQUIRE( queues[1] == std::max(&queue1, &queue2) );

    REQUIRE_CALL(queue1, wait_until_completed());
    REQUIRE_CALL(queue2, wait_until_completed());
    buffer.reset();
}

TEST_CASE( "recording the allocation queue to a caching_buffer_sentinel should not store it" )
{
    const std::size_t max_alignment = 256;
    const std::size_t request_size_step = 1<<20;
    mock_memory_resource resource;

    REQUIRE_CALL(resource, get_target_device())
        .RETURN(nullptr);
    caching_memory_allocator allocator(
        resource, 
        max_alignment, 
        request_size_step
    );

    mock_device_queue queue;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(request_size_step);
    REQUIRE_CALL(*heap, create_buffer(0, 256, ANY(std::unique_ptr<buffer_sentinel>)))
        .LR_RETURN(std::make_shared<buffer>(nullptr, 256, resource, std::move(_3)));
    REQUIRE_CALL(resource, create_memory_heap(request_size_step, max_alignment))
        .RETURN(heap);

    const auto buffer = allocator.allocate(20, 32, &queue);
    auto* sentinel = dynamic_cast<caching_buffer_sentinel*>(buffer->get_sentinel());

    sentinel->record_queue(queue, false);

    const auto queues = sentinel->get_queues();
    REQUIRE( queues.empty() );
}

TEST_CASE( "repeatedly adding the same queue to a caching_buffer_sentinel should only store it once" )
{
    const std::size_t max_alignment = 256;
    const std::size_t request_size_step = 1<<20;
    mock_memory_resource resource;

    REQUIRE_CALL(resource, get_target_device())
        .RETURN(nullptr);
    caching_memory_allocator allocator(
        resource, 
        max_alignment, 
        request_size_step
    );

    mock_device_queue queue;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(request_size_step);
    REQUIRE_CALL(*heap, create_buffer(0, 256, ANY(std::unique_ptr<buffer_sentinel>)))
        .LR_RETURN(std::make_shared<buffer>(nullptr, 256, resource, std::move(_3)));
    REQUIRE_CALL(resource, create_memory_heap(request_size_step, max_alignment))
        .RETURN(heap);

    auto buffer = allocator.allocate(20, 32, nullptr);
    auto* sentinel = dynamic_cast<caching_buffer_sentinel*>(buffer->get_sentinel());

    sentinel->record_queue(queue, false);
    sentinel->record_queue(queue, false);

    const auto queues = sentinel->get_queues();
    REQUIRE( queues.size() == 1 );
    REQUIRE( queues[0] == &queue );

    REQUIRE_CALL(queue, wait_until_completed());
    buffer.reset();
}

TEST_CASE( "adding a queue with the exclusive flag to a caching_buffer_sentinel should erase previous queues" )
{
    const std::size_t max_alignment = 256;
    const std::size_t request_size_step = 1<<20;
    mock_memory_resource resource;

    REQUIRE_CALL(resource, get_target_device())
        .RETURN(nullptr);
    caching_memory_allocator allocator(
        resource, 
        max_alignment, 
        request_size_step
    );

    mock_device_queue queue1;
    mock_device_queue queue2;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(request_size_step);
    REQUIRE_CALL(*heap, create_buffer(0, 256, ANY(std::unique_ptr<buffer_sentinel>)))
        .LR_RETURN(std::make_shared<buffer>(nullptr, 256, resource, std::move(_3)));
    REQUIRE_CALL(resource, create_memory_heap(request_size_step, max_alignment))
        .RETURN(heap);

    auto buffer = allocator.allocate(20, 32, nullptr);
    auto* sentinel = dynamic_cast<caching_buffer_sentinel*>(buffer->get_sentinel());

    sentinel->record_queue(queue1, false);
    sentinel->record_queue(queue2, true);

    const auto queues = sentinel->get_queues();
    REQUIRE( queues.size() == 1 );
    REQUIRE( queues[0] == &queue2 );

    REQUIRE_CALL(queue2, wait_until_completed());
    buffer.reset();
}
