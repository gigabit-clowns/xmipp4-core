// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <hardware/caching_memory_allocator/caching_memory_allocator.hpp>

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/buffer_sentinel.hpp>

#include "../mock/mock_memory_resource.hpp"
#include "../mock/mock_memory_heap.hpp"

#include <stdexcept>

using namespace xmipp4::hardware;

TEST_CASE( "requesting a buffer from caching_memory_allocator should allocate a heap the first time with more than enough space", "[caching_memory_allocator_backend]" )
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

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(request_size_step);
    REQUIRE_CALL(*heap, create_buffer(0, 256, ANY(std::unique_ptr<buffer_sentinel>)))
        .LR_RETURN(std::make_shared<buffer>(nullptr, 256, resource, std::move(_3)));
    REQUIRE_CALL(resource, create_memory_heap(request_size_step, max_alignment))
        .RETURN(heap);

    allocator.allocate(20, 32, nullptr);
}

TEST_CASE( "repeatedly requesting buffer from caching_memory_allocator should re-use resources", "[caching_memory_allocator_backend]" )
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

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(request_size_step);

    REQUIRE_CALL(resource, create_memory_heap(request_size_step, max_alignment))
        .RETURN(heap);

    for (std::size_t i = 0; i < 1000; ++i)
    {
        REQUIRE_CALL(*heap, create_buffer(0, 256, ANY(std::unique_ptr<buffer_sentinel>)))
            .LR_RETURN(std::make_shared<buffer>(nullptr, 256, resource, std::move(_3)));
        allocator.allocate(20, 32, nullptr);
    }
}

TEST_CASE( "requesting a buffer from caching_memory_allocator should leverage existing resources", "[caching_memory_allocator_backend]" )
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

    auto heap = std::make_shared<mock_memory_heap>();
    REQUIRE_CALL(*heap, get_size())
        .RETURN(request_size_step);

    REQUIRE_CALL(resource, create_memory_heap(request_size_step, max_alignment))
        .RETURN(heap);

    std::array<std::shared_ptr<buffer>, 4> buffers;
    for (std::size_t i = 0; i < buffers.size(); ++i)
    {
        REQUIRE_CALL(*heap, create_buffer(max_alignment*i, 256, ANY(std::unique_ptr<buffer_sentinel>)))
            .LR_RETURN(std::make_shared<buffer>(nullptr, 256, resource, std::move(_3)));
        buffers[i] = allocator.allocate(20, 32, nullptr);
    }
}

TEST_CASE( "requesting an over-aligned buffer from caching_memory_allocator should throw", "[caching_memory_allocator_backend]" )
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

    REQUIRE_THROWS_MATCHES(
        allocator.allocate(20, max_alignment*2, nullptr),
		std::invalid_argument,
		Catch::Matchers::Message(
            "alignment parameter exceeds the maximum alignment of this "
            "allocator"
        )
	);
}

TEST_CASE( "requesting a buffer from caching_allocator should retry after freeing resources when an allocation fails", "[caching_memory_allocator_backend]" )
{
    // TODO
}

TEST_CASE( "requesting a buffer from caching_allocator should throw when allocation fails repeatedly", "[caching_memory_allocator_backend]" )
{
    // TODO
}