// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/hardware/memory_allocator_manager.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>

#include "mock/mock_memory_allocator_backend.hpp"
#include "mock/mock_memory_allocator.hpp"
#include "mock/mock_memory_resource.hpp"


using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE( "memory_allocator_manager should allow registering backends", "[memory_allocator_backend]" )
{
    memory_allocator_manager manager;

    REQUIRE( manager.register_backend(std::move(std::make_unique<mock_memory_allocator_backend>())) == true );
    REQUIRE( manager.register_backend(std::move(std::make_unique<mock_memory_allocator_backend>())) == true );
}

TEST_CASE( "memory_allocator_manager does not allow registering null backend", "[memory_allocator_backend]" )
{
    memory_allocator_manager manager;

    REQUIRE( manager.register_backend( nullptr ) == false );
}

// TODO other tests
