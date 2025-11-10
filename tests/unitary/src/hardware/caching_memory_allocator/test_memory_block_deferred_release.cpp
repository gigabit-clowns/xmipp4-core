// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <hardware/caching_memory_allocator/memory_block_deferred_release.hpp>

#include <xmipp4/core/hardware/buffer_sentinel.hpp>

#include "../mock/mock_device.hpp"
#include "../mock/mock_device_queue.hpp"
#include "../mock/mock_device_to_host_event.hpp"
#include "../mock/mock_memory_heap.hpp"

#include <cstddef>


using namespace xmipp4::hardware;

TEST_CASE( "deferring a release in memory_block_deferred_release without queues should throw", "[caching_memory_allocator]" )
{
    memory_block_pool pool;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    auto ite = pool.register_heap(std::move(heap), nullptr);

    mock_device device;

    memory_block_deferred_release defer;


    REQUIRE_THROWS_MATCHES(
        defer.defer_release(ite, {}, device),
		std::invalid_argument,
		Catch::Matchers::Message("No queues were provided to defer the release")
	);
}

TEST_CASE( "deferring a release in memory_block_deferred_release with a null queue should throw", "[caching_memory_allocator]" )
{
    memory_block_pool pool;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    auto ite = pool.register_heap(std::move(heap), nullptr);

    mock_device device;

    memory_block_deferred_release defer;

    const std::array<device_queue*, 1> queues = {
        nullptr
    };
    REQUIRE_THROWS_MATCHES(
        defer.defer_release(ite, xmipp4::make_span(queues), device),
		std::invalid_argument,
		Catch::Matchers::Message("nullptr queue was provided")
	);
}

#if !XMIPP4_MSVC // FIXME: For some reason IN_SEQUENCE does not work with MSVC

TEST_CASE( "deferring a release in memory_block_deferred_release should create and signal an event per queue", "[caching_memory_allocator]" )
{
    memory_block_pool pool;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    auto ite = pool.register_heap(std::move(heap), nullptr);

    mock_device device;
    mock_device_queue queue1;
    mock_device_queue queue2;
    auto event1 = std::make_shared<mock_device_to_host_event>();
    auto event2 = std::make_shared<mock_device_to_host_event>();
    trompeloeil::sequence seq;

    REQUIRE_CALL(device, create_device_to_host_event())
        .IN_SEQUENCE(seq)
        .RETURN(event1);
    REQUIRE_CALL(*event1, signal(ANY(device_queue&)))
        .LR_WITH(&_1 == &queue1)
        .IN_SEQUENCE(seq);
    REQUIRE_CALL(device, create_device_to_host_event())
        .IN_SEQUENCE(seq)
        .RETURN(event2);
    REQUIRE_CALL(*event2, signal(ANY(device_queue&)))
        .LR_WITH(&_1 == &queue2)
        .IN_SEQUENCE(seq);

    memory_block_deferred_release defer;    
    const std::array<device_queue*, 2> queues = {
        &queue1,
        &queue2
    };
    
    defer.defer_release(ite, xmipp4::make_span(queues), device);
}

TEST_CASE( "processing pending frees in memory_block_deferred release should mark blocks as free only when completed in all queues", "[caching_memory_allocator]" )
{
    memory_block_pool pool;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    auto ite = pool.register_heap(std::move(heap), nullptr);
    ite->second.set_free(false);

    mock_device device;
    mock_device_queue queue1;
    mock_device_queue queue2;
    auto event1 = std::make_shared<mock_device_to_host_event>();
    auto event2 = std::make_shared<mock_device_to_host_event>();

    memory_block_deferred_release defer;    

    {
        trompeloeil::sequence seq;

        REQUIRE_CALL(device, create_device_to_host_event())
            .IN_SEQUENCE(seq)
            .RETURN(event1);
        REQUIRE_CALL(*event1, signal(ANY(device_queue&)))
            .LR_WITH(&_1 == &queue1)
            .IN_SEQUENCE(seq);
        REQUIRE_CALL(device, create_device_to_host_event())
            .IN_SEQUENCE(seq)
            .RETURN(event2);
        REQUIRE_CALL(*event2, signal(ANY(device_queue&)))
            .LR_WITH(&_1 == &queue2)
            .IN_SEQUENCE(seq);

        const std::array<device_queue*, 2> queues = {
            &queue1,
            &queue2
        };
        
        defer.defer_release(ite, xmipp4::make_span(queues), device);
    }

    SECTION("No completion")
    {
        REQUIRE_CALL(*event1, is_signaled())
            .RETURN(false);
        REQUIRE_CALL(*event2, is_signaled())
            .RETURN(false);

        defer.process_pending_free(pool);
        REQUIRE( ite->second.is_free() == false );
    }

    SECTION("Partial completion")
    {
        REQUIRE_CALL(*event1, is_signaled())
            .RETURN(true);
        REQUIRE_CALL(*event2, is_signaled())
            .RETURN(false);

        defer.process_pending_free(pool);
        REQUIRE( ite->second.is_free() == false );
    }

    SECTION("Full completion")
    {
        REQUIRE_CALL(*event1, is_signaled())
            .RETURN(true);
        REQUIRE_CALL(*event2, is_signaled())
            .RETURN(true);

        defer.process_pending_free(pool);
        REQUIRE( ite->second.is_free() == true ); // Freed!
    }
}

TEST_CASE( "waiting pending frees in memory_block_deferred release should wait for all events", "[caching_memory_allocator]" )
{
    memory_block_pool pool;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    auto ite = pool.register_heap(std::move(heap), nullptr);
    ite->second.set_free(false);

    mock_device device;
    mock_device_queue queue1;
    mock_device_queue queue2;
    auto event1 = std::make_shared<mock_device_to_host_event>();
    auto event2 = std::make_shared<mock_device_to_host_event>();

    memory_block_deferred_release defer;    

    {
        trompeloeil::sequence seq;

        REQUIRE_CALL(device, create_device_to_host_event())
            .IN_SEQUENCE(seq)
            .RETURN(event1);
        REQUIRE_CALL(*event1, signal(ANY(device_queue&)))
            .LR_WITH(&_1 == &queue1)
            .IN_SEQUENCE(seq);
        REQUIRE_CALL(device, create_device_to_host_event())
            .IN_SEQUENCE(seq)
            .RETURN(event2);
        REQUIRE_CALL(*event2, signal(ANY(device_queue&)))
            .LR_WITH(&_1 == &queue2)
            .IN_SEQUENCE(seq);

        const std::array<device_queue*, 2> queues = {
            &queue1,
            &queue2
        };
        
        defer.defer_release(ite, xmipp4::make_span(queues), device);
    }

    REQUIRE_CALL(*event1, wait());
    REQUIRE_CALL(*event2, wait());

    defer.wait_pending_free(pool);
    REQUIRE( ite->second.is_free() == true );
}

TEST_CASE( "repeated use cycle of memory_block_deferred release should reuse its resources", "[caching_memory_allocator]" )
{
    memory_block_pool pool;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    auto ite = pool.register_heap(std::move(heap), nullptr);

    mock_device device;
    mock_device_queue queue;
    auto event = std::make_shared<mock_device_to_host_event>();

    memory_block_deferred_release defer;
    const std::array<device_queue*, 1> queues = { &queue };

    {
        trompeloeil::sequence seq;

        REQUIRE_CALL(device, create_device_to_host_event())
            .IN_SEQUENCE(seq)
            .RETURN(event);
        REQUIRE_CALL(*event, signal(ANY(device_queue&)))
            .LR_WITH(&_1 == &queue)
            .IN_SEQUENCE(seq);

        defer.defer_release(ite, xmipp4::make_span(queues), device);
    }
    {
        trompeloeil::sequence seq;

        REQUIRE_CALL(*event, is_signaled())
            .IN_SEQUENCE(seq)
            .RETURN(true);

        defer.process_pending_free(pool);
        REQUIRE( ite->second.is_free() );
    }
    {
        trompeloeil::sequence seq;

        REQUIRE_CALL(*event, signal(ANY(device_queue&)))
            .LR_WITH(&_1 == &queue)
            .IN_SEQUENCE(seq);

        defer.defer_release(ite, xmipp4::make_span(queues), device);
    }
    {
        trompeloeil::sequence seq;

        REQUIRE_CALL(*event, wait())
            .IN_SEQUENCE(seq);

        defer.wait_pending_free(pool);
        REQUIRE( ite->second.is_free() );
    }
}

#endif // !XMIPP4_MSVC
