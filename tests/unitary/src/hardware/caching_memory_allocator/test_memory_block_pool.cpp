// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <hardware/caching_memory_allocator/memory_block_pool.hpp>

#include <xmipp4/core/hardware/buffer_sentinel.hpp>

#include "../mock/mock_device_queue.hpp"
#include "../mock/mock_memory_heap.hpp"

#include <numeric>

using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE("find_suitable_block should return the best fit for the requested size and queue")
{
    memory_block_pool pool;

    mock_device_queue queue1;
    mock_device_queue queue2;

    using param_tuple = std::tuple<std::size_t, device_queue*, bool>;

    std::array<param_tuple, 6> params = {
        param_tuple(1024, &queue1, true),
        param_tuple(96, nullptr, true),
        param_tuple(96, &queue1, false),
        param_tuple(100, &queue1, true),
        param_tuple(96, &queue2, true),
        param_tuple(64, &queue1, true),
    };

    for (const auto &param : params)
    {
        std::size_t size;
        device_queue *queue;
        bool free;
        std::tie(size, queue, free) = param;
        
        auto heap = std::make_shared<mock_memory_heap>();
        REQUIRE_CALL(*heap, get_size())
            .RETURN(size);

        auto ite = pool.register_heap(
            std::move(heap),
            queue
        );

        if (!free)
        {
            ite->second.set_free(false);
        }
    }

    memory_block_pool::iterator block;

    block = pool.find_suitable_block(96, &queue1);
    REQUIRE( block->first.get_size() == 100 );
    REQUIRE( block->first.get_queue() == &queue1 );

    block = pool.find_suitable_block(96, &queue2);
    REQUIRE( block->first.get_size() == 96 );
    REQUIRE( block->first.get_queue() == &queue2 );

    block = pool.find_suitable_block(96, nullptr);
    REQUIRE( block->first.get_size() == 96 );
    REQUIRE( block->first.get_queue() == nullptr );

    block = pool.find_suitable_block(1024, &queue1);
    REQUIRE( block->first.get_size() == 1024 );
    REQUIRE( block->first.get_queue() == &queue1 );

    block = pool.find_suitable_block(32, &queue1);
    REQUIRE( block->first.get_size() == 64 );
    REQUIRE( block->first.get_queue() == &queue1 );

    block = pool.find_suitable_block(90, nullptr);
    REQUIRE( block->first.get_size() == 96 );
    REQUIRE( block->first.get_queue() == nullptr );
}

TEST_CASE("find_suitable_block should return end() when no block fits the requirement")
{
    memory_block_pool pool;

    mock_device_queue queue1;
    mock_device_queue queue2;

    using param_tuple = std::tuple<std::size_t, device_queue*, bool>;

    std::array<param_tuple, 6> params = {
        param_tuple(512, &queue1, true),
        param_tuple(1024, &queue1, false),
        param_tuple(1024, &queue2, true),
        param_tuple(2048, &queue2, false),
        param_tuple(1024, nullptr, true),
        param_tuple(2048, nullptr, false),
    };

    for (const auto &param : params)
    {
        std::size_t size;
        device_queue *queue;
        bool free;
        std::tie(size, queue, free) = param;
        
        auto heap = std::make_shared<mock_memory_heap>();
        REQUIRE_CALL(*heap, get_size())
            .RETURN(size);

        auto ite = pool.register_heap(
            std::move(heap),
            queue
        );

        if (!free)
        {
            ite->second.set_free(false);
        }
    }

    REQUIRE( pool.find_suitable_block(1024, &queue1) == pool.end() );
    REQUIRE( pool.find_suitable_block(768, &queue1) == pool.end() );
    REQUIRE( pool.find_suitable_block(2048, &queue1) == pool.end() );
    REQUIRE( pool.find_suitable_block(1025, &queue2) == pool.end() );
    REQUIRE( pool.find_suitable_block(1025, nullptr) == pool.end() );
}

TEST_CASE("partition_block should create a valid pair of memory_blocks", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);

    mock_device_queue queue;
    auto ite = pool.register_heap(heap, &queue);

    const std::size_t first_size = 256;
    const std::size_t second_size = 768;
    XMIPP4_ASSERT( first_size + second_size == size );

    memory_block_pool::iterator first;
    memory_block_pool::iterator second;
    std::tie(first, second) = pool.partition_block(ite, first_size, second_size);

    REQUIRE( heap.use_count() == 3 );

    REQUIRE( first->first.get_heap() == heap.get() );
    REQUIRE( first->first.get_offset() == 0 );
    REQUIRE( first->first.get_size() == first_size );
    REQUIRE( first->first.get_queue() == &queue );
    REQUIRE( first->second.is_free() == true );
    REQUIRE( first->second.get_previous_block() == pool.end() );
    REQUIRE( first->second.get_next_block() == second );

    REQUIRE( second->first.get_heap() == heap.get() );
    REQUIRE( second->first.get_offset() == first_size );
    REQUIRE( second->first.get_size() == second_size );
    REQUIRE( second->first.get_queue() == &queue );
    REQUIRE( second->second.is_free() == true );
    REQUIRE( second->second.get_previous_block() == first );
    REQUIRE( second->second.get_next_block() == pool.end() );
}

TEST_CASE("successive calls to partition_block should create a doubly linked list of memory_blocks", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);

    mock_device_queue queue;
    auto ite = pool.register_heap(heap, &queue);

    const std::size_t n_partitions = 8;
    const std::size_t partition_size = size / n_partitions;
    std::vector<memory_block_pool::iterator> partitions;
    partitions.emplace_back(pool.end());
    for (std::size_t i = 1; i < n_partitions; ++i)
    {
        memory_block_pool::iterator first;
        const auto remaining = ite->first.get_size() - partition_size;
        std::tie(first, ite) = pool.partition_block(
            ite, 
            partition_size, 
            remaining
        );

        partitions.emplace_back(first);
    }
    partitions.emplace_back(ite);
    partitions.emplace_back(pool.end());

    REQUIRE( partitions.size() == n_partitions+2 );
    for (std::size_t i = 1; i <= n_partitions; ++i)
    {
        const auto prev = partitions[i]->second.get_previous_block();
        const auto next = partitions[i]->second.get_next_block();

        REQUIRE( prev == partitions[i-1] );
        REQUIRE( next == partitions[i+1] );
    }
}

TEST_CASE("register_heap should store the provided heap in the resulting block", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);

    auto ite = pool.register_heap(heap, nullptr);
    REQUIRE( ite->first.get_heap() == heap.get() );
}

TEST_CASE("register_heap should return a block with zero offset", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);

    auto ite = pool.register_heap(heap, nullptr);
    REQUIRE( ite->first.get_offset() == 0 );
}

TEST_CASE("register_heap should query the size of the heap and store it in the resulting block", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);

    auto ite = pool.register_heap(std::move(heap), nullptr);
    REQUIRE( ite->first.get_size() == size );
}

TEST_CASE("register_heap should store the provided queue in the resulting block", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);

    mock_device_queue queue;

    auto ite = pool.register_heap(std::move(heap), &queue);
    REQUIRE( ite->first.get_queue() == &queue );
}

TEST_CASE("register_heap should return a free bock", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);

    auto ite = pool.register_heap(heap, nullptr);
    REQUIRE( ite->second.is_free() == true );
}

TEST_CASE("register_heap should return an unpartitioned block", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);

    auto ite = pool.register_heap(heap, nullptr);
    REQUIRE( ite->second.get_next_block() == pool.end() );
    REQUIRE( ite->second.get_previous_block() == pool.end() );
}

TEST_CASE("consider_merging_blocks should not do anything if the block is not a partition", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    auto ite = pool.register_heap(heap, nullptr);

    const auto old_block = *ite;
    pool.consider_merging_block(ite);
    REQUIRE( *ite == old_block );
}

// TODO add tests for consider_merging_blocks

TEST_CASE("release_blocks should release free heaps", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    pool.register_heap(heap, nullptr);
    
    REQUIRE( heap.use_count() == 2 );
    pool.release_blocks();
    REQUIRE( heap.use_count() == 1 );
}

TEST_CASE("release_blocks should not release if it is not free", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    auto ite = pool.register_heap(heap, nullptr);
    ite->second.set_free(false);
    
    REQUIRE( heap.use_count() == 2 );
    pool.release_blocks();
    REQUIRE( heap.use_count() == 2 );
}

TEST_CASE("release_blocks should not release if it has a next block", "[memory_block_pool]")
{
    memory_block_pool pool;
    
    const std::size_t size = 1024;
    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(1024);
    auto ite = pool.register_heap(heap, nullptr);
    pool.partition_block(ite, 512, 512);

    REQUIRE( heap.use_count() == 3 );
    pool.release_blocks();
    REQUIRE( heap.use_count() == 3 );
}
