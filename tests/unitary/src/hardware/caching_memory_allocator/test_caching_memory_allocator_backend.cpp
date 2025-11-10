// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <hardware/caching_memory_allocator/caching_memory_allocator_backend.hpp>

#include <hardware/host_memory/host_memory_resource.hpp>

#include "../mock/mock_memory_resource.hpp"

#include <sstream>


using namespace xmipp4::hardware;

TEST_CASE( "caching_memory_allocator_backend should always have normal priority", "[caching_memory_allocator_backend]" )
{
    caching_memory_allocator_backend backend;

    mock_memory_resource resource;
    REQUIRE( backend.get_suitability(resource) == xmipp4::backend_priority::normal );
}

TEST_CASE( "caching_memory_allocator_backend should always return a valid allocator", "[caching_memory_allocator_backend]" )
{
    caching_memory_allocator_backend backend;

    mock_memory_resource resource;
    REQUIRE_CALL(resource, get_target_device())
        .RETURN(nullptr);

    auto allocator = backend.create_memory_allocator(resource);

    REQUIRE( allocator != nullptr );
}
