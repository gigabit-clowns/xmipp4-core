// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <hardware/caching_memory_allocator/memory_block_pool.hpp> // Include before
#include <hardware/caching_memory_allocator/memory_block_context.hpp>

#include <xmipp4/core/hardware/buffer_sentinel.hpp>

#include "../mock/mock_memory_heap.hpp"

#include <cstddef>


using namespace xmipp4::hardware;

TEST_CASE( "Constructing a memory_block_context should store the arguments", "[caching_memory_allocator]" )
{
    memory_block_pool pool;

    std::shared_ptr<mock_memory_heap> heap;
    heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    const auto ite1 = pool.register_heap(std::move(heap), nullptr);
    heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    const auto ite2 = pool.register_heap(std::move(heap), nullptr);

    memory_block_context context(ite1, ite2, true);

    REQUIRE( context.get_previous_block() == ite1 );
    REQUIRE( context.get_next_block() == ite2 );
    REQUIRE( context.is_free() == true );
}

TEST_CASE( "setting the previous memory block in memory_block_context should store it", "[caching_memory_allocator]" )
{
    memory_block_pool pool;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    const auto ite = pool.register_heap(std::move(heap), nullptr);


    memory_block_context context(
        memory_block_pool::iterator(), 
        memory_block_pool::iterator(), 
        true
    );

    context.set_previous_block(ite);
    REQUIRE( context.get_previous_block() == ite );
}

TEST_CASE( "setting the next memory block in memory_block_context should store it", "[caching_memory_allocator]" )
{
    memory_block_pool pool;

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    const auto ite = pool.register_heap(std::move(heap), nullptr);


    memory_block_context context(
        memory_block_pool::iterator(), 
        memory_block_pool::iterator(), 
        true
    );

    context.set_next_block(ite);
    REQUIRE( context.get_next_block() == ite );
}

TEST_CASE( "setting the free flag in memory_block_context should store it", "[caching_memory_allocator]" )
{
    memory_block_pool pool;

    memory_block_context context(
        memory_block_pool::iterator(), 
        memory_block_pool::iterator(), 
        true
    );

    context.set_free(false);
    REQUIRE( context.is_free() == false );
    context.set_free(true);
    REQUIRE( context.is_free() == true );
}
