// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <hardware/caching_memory_allocator/memory_block_pool.hpp>

#include <xmipp4/core/hardware/buffer_sentinel.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include "../mock/mock_device_queue.hpp"
#include "../mock/mock_memory_heap.hpp"

#include <numeric>

using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE( "acquire in memory_block_pool should mark the block as not free", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);
	mock_device_queue queue;
	auto *block = pool.register_heap(heap, &queue);

	REQUIRE( block->is_free() == true );
	pool.acquire(*block);
	REQUIRE( block->is_free() == false );
}

TEST_CASE( "release in memory_block_pool should mark the block as free", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);
	mock_device_queue queue;
	auto *block = pool.register_heap(heap, &queue);

	pool.acquire(*block);
	REQUIRE( block->is_free() == false );
	pool.release(*block);
	REQUIRE( block->is_free() == true );
}

TEST_CASE("find_suitable_block should return the best fit for the requested size and queue", "[memory_block_pool]")
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

		auto *block = pool.register_heap(
			std::move(heap),
			queue
		);

		if (!free)
		{
			pool.acquire(*block);
		}
	}

	memory_block *block;

	block = pool.find_suitable_block(96, &queue1);
	REQUIRE( block->get_size() == 100 );
	REQUIRE( block->get_queue() == &queue1 );

	block = pool.find_suitable_block(96, &queue2);
	REQUIRE( block->get_size() == 96 );
	REQUIRE( block->get_queue() == &queue2 );

	block = pool.find_suitable_block(96, nullptr);
	REQUIRE( block->get_size() == 96 );
	REQUIRE( block->get_queue() == nullptr );

	block = pool.find_suitable_block(1024, &queue1);
	REQUIRE( block->get_size() == 1024 );
	REQUIRE( block->get_queue() == &queue1 );

	block = pool.find_suitable_block(32, &queue1);
	REQUIRE( block->get_size() == 64 );
	REQUIRE( block->get_queue() == &queue1 );

	block = pool.find_suitable_block(90, nullptr);
	REQUIRE( block->get_size() == 96 );
	REQUIRE( block->get_queue() == nullptr );
}

TEST_CASE("find_suitable_block should return nullptr when no block fits the requirement", "[memory_block_pool]")
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

		auto *block = pool.register_heap(
			std::move(heap),
			queue
		);

		if (!free)
		{
			pool.acquire(*block);
		}
	}

	REQUIRE( pool.find_suitable_block(1024, &queue1) == nullptr );
	REQUIRE( pool.find_suitable_block(768, &queue1) == nullptr );
	REQUIRE( pool.find_suitable_block(2048, &queue1) == nullptr );
	REQUIRE( pool.find_suitable_block(1025, &queue2) == nullptr );
	REQUIRE( pool.find_suitable_block(1025, nullptr) == nullptr );
}

TEST_CASE("partition_block should create a valid pair of memory_blocks", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);

	mock_device_queue queue;
	auto *block = pool.register_heap(heap, &queue);

	const std::size_t first_size = 256;
	const std::size_t second_size = 768;
	XMIPP4_ASSERT( first_size + second_size == size );

	memory_block *first;
	memory_block *second;
	std::tie(first, second) = pool.partition_block(block, first_size, second_size);

	REQUIRE( first->get_heap() == heap.get() );
	REQUIRE( first->get_offset() == 0 );
	REQUIRE( first->get_size() == first_size );
	REQUIRE( first->get_queue() == &queue );
	REQUIRE( first->is_free());

	REQUIRE( second->get_heap() == heap.get() );
	REQUIRE( second->get_offset() == first_size );
	REQUIRE( second->get_size() == second_size );
	REQUIRE( second->get_queue() == &queue );
	REQUIRE( second->is_free() );
}

TEST_CASE("register_heap should store the provided heap in the resulting block", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);

	auto *block = pool.register_heap(heap, nullptr);
	REQUIRE( block->get_heap() == heap.get() );
}

TEST_CASE("register_heap should return a block with zero offset", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);

	auto *block = pool.register_heap(heap, nullptr);
	REQUIRE( block->get_offset() == 0 );
}

TEST_CASE("register_heap should query the size of the heap and store it in the resulting block", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);

	auto *block = pool.register_heap(std::move(heap), nullptr);
	REQUIRE( block->get_size() == size );
}

TEST_CASE("register_heap should store the provided queue in the resulting block", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);

	mock_device_queue queue;

	auto *block = pool.register_heap(std::move(heap), &queue);
	REQUIRE( block->get_queue() == &queue );
}

TEST_CASE("register_heap should return a free bock", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);

	auto *block = pool.register_heap(heap, nullptr);
	REQUIRE( block->is_free() );
}

TEST_CASE("register_heap should increment the reference count by one", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);

	auto ite = pool.register_heap(heap, nullptr);
	REQUIRE( heap.use_count() == 2 );
}

TEST_CASE("consider_merging_blocks should not do anything if the block is not a partition", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	mock_device_queue queue;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);
	auto *block = pool.register_heap(heap, &queue);

	const auto old_block = *block;
	pool.consider_merging_block(*block);
	REQUIRE( *block == old_block );
}

TEST_CASE("consider_merging_blocks should not do anything if the block has occupied partitions", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	mock_device_queue queue;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);
	auto *block = pool.register_heap(heap, &queue);

	memory_block* partition;
	std::tie(block, partition) = pool.partition_block(block, 768, 256);
	pool.acquire(*partition);
	std::tie(partition, block) = pool.partition_block(block, 256, 512);
	pool.acquire(*partition);

	const auto old_block = *block;
	pool.consider_merging_block(*block);
	REQUIRE( *block == old_block );
}

TEST_CASE("consider_merging_blocks should merge when there is a free partition to the right", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	mock_device_queue queue;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);
	auto *block = pool.register_heap(heap, &queue);

	memory_block *partition;
	std::tie(block, partition) = pool.partition_block(block, 768, 256);
	std::tie(partition, block) = pool.partition_block(block, 256, 512);
	pool.acquire(*partition);

	pool.consider_merging_block(*block);
	REQUIRE( block->get_heap() == heap.get() );
	REQUIRE( block->get_offset() == 256 );
	REQUIRE( block->get_size() == 768 );
	REQUIRE( block->get_queue() == &queue );
	REQUIRE( block->is_free() );
}

TEST_CASE("consider_merging_blocks should merge when there is a free partition to the left", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	mock_device_queue queue;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);
	auto *block = pool.register_heap(heap, &queue);

	memory_block *next;
	std::tie(block, next) = pool.partition_block(block, 768, 256);
	pool.acquire(*next);

	memory_block *prev;
	std::tie(prev, block) = pool.partition_block(block, 256, 512);

	pool.consider_merging_block(*block);
	REQUIRE( block->get_heap() == heap.get() );
	REQUIRE( block->get_offset() == 0 );
	REQUIRE( block->get_size() == 768 );
	REQUIRE( block->get_queue() == &queue );
	REQUIRE( block->is_free() );
}

TEST_CASE("consider_merging_blocks should merge twice when there is a free partition to the right and left", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	mock_device_queue queue;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);
	auto *block = pool.register_heap(heap, &queue);

	std::tie(block, std::ignore) = pool.partition_block(block, 768, 256);
	std::tie(std::ignore, block) = pool.partition_block(block, 256, 512);

	pool.consider_merging_block(*block);
	REQUIRE( block->get_heap() == heap.get() );
	REQUIRE( block->get_offset() == 0 );
	REQUIRE( block->get_size() == 1024 );
	REQUIRE( block->get_queue() == &queue );
	REQUIRE( block->is_free() );
}

TEST_CASE("release_unused_heaps should release free heaps", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);
	pool.register_heap(heap, nullptr);
	
	REQUIRE( heap.use_count() == 2 );
	pool.release_unused_heaps();
	REQUIRE( heap.use_count() == 1 );
}

TEST_CASE("release_unused_heaps should not release if it is not free", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);
	auto *block = pool.register_heap(heap, nullptr);
	pool.acquire(*block);
	
	REQUIRE( heap.use_count() == 2 );
	pool.release_unused_heaps();
	REQUIRE( heap.use_count() == 2 );
}

TEST_CASE("release_unused_heaps should not release if it has a next block", "[memory_block_pool]")
{
	memory_block_pool pool;
	
	const std::size_t size = 1024;
	auto heap = std::make_shared<mock_memory_heap>();
	REQUIRE_CALL(*heap, get_size())
		.RETURN(1024);
	auto *block = pool.register_heap(heap, nullptr);
	pool.partition_block(block, 512, 512);

	REQUIRE( heap.use_count() == 2 );
	pool.release_unused_heaps();
	REQUIRE( heap.use_count() == 2 );
}
