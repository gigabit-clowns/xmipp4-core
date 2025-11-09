// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <hardware/host_memory/host_memory_resource.hpp>

#include <xmipp4/core/hardware/memory_heap.hpp>

using namespace xmipp4::hardware;

TEST_CASE( "host_memory_resource should be a singleton", "[host_memory_resource]" )
{
    auto &a = host_memory_resource::get();
    auto &b = host_memory_resource::get();
    REQUIRE( &a == &b );
}

TEST_CASE( "host_memory_resource should not target any device", "[host_memory_resource]" )
{
    const auto& resource = host_memory_resource::get();
    REQUIRE( resource.get_target_device() == nullptr );
}

TEST_CASE( "host_memory_resource should be of host kind", "[host_memory_resource]" )
{
    const auto& resource = host_memory_resource::get();
    REQUIRE( resource.get_kind() == memory_resource_kind::host );
}

TEST_CASE( "host_memory_resource should create a valid heap", "[host_memory_resource]" )
{
    auto& resource = host_memory_resource::get();

    const std::size_t size = 1024;
    const std::size_t alignment = 16;
    auto heap = resource.create_memory_heap(size, alignment);

    REQUIRE( heap );
    REQUIRE( heap->get_size() >= size );
}
