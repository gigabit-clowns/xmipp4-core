// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include "core/hardware/memory_allocator_pool.hpp"

#include "mock/mock_memory_allocator.hpp"
#include "mock/mock_memory_resource.hpp"

using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE(
    "memory_allocator_pool touch should create an allocator "
    "on first access",
    "[memory_allocator_pool]"
)
{
    auto allocator = std::make_shared<mock_memory_allocator>();
    mock_memory_resource resource;
    REQUIRE_CALL(resource, create_allocator())
        .RETURN(allocator);

    memory_allocator_pool pool;
    const auto& result = pool.touch(resource);

    REQUIRE( result == allocator );
}

TEST_CASE(
    "memory_allocator_pool touch should return the cached allocator "
    "on subsequent accesses to the same resource",
    "[memory_allocator_pool]"
)
{
    auto allocator = std::make_shared<mock_memory_allocator>();
    mock_memory_resource resource;
    REQUIRE_CALL(resource, create_allocator())
        .RETURN(allocator)
        .TIMES(1);

    memory_allocator_pool pool;
    const auto& first  = pool.touch(resource);
    const auto& second = pool.touch(resource);

    REQUIRE( first  == allocator );
    REQUIRE( second == allocator );
    REQUIRE( &first == &second );
}

TEST_CASE(
    "memory_allocator_pool touch should create independent allocators "
    "for different resources",
    "[memory_allocator_pool]"
)
{
    auto allocator1 = std::make_shared<mock_memory_allocator>();
    auto allocator2 = std::make_shared<mock_memory_allocator>();
    mock_memory_resource resource1;
    mock_memory_resource resource2;
    REQUIRE_CALL(resource1, create_allocator())
        .RETURN(allocator1);
    REQUIRE_CALL(resource2, create_allocator())
        .RETURN(allocator2);

    memory_allocator_pool pool;
    const auto& result1 = pool.touch(resource1);
    const auto& result2 = pool.touch(resource2);

    REQUIRE( result1 == allocator1 );
    REQUIRE( result2 == allocator2 );
    REQUIRE( result1 != result2 );
}
