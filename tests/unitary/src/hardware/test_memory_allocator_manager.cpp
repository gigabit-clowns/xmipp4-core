// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <trompeloeil/sequence.hpp>

#include <xmipp4/core/hardware/memory_allocator_manager.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "mock/mock_memory_allocator_backend.hpp"
#include "mock/mock_memory_allocator.hpp"
#include "mock/mock_memory_resource.hpp"


using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE( "memory_allocator_manager should allow registering backends", "[memory_allocator_backend]" )
{
    memory_allocator_manager manager;

    REQUIRE( manager.register_backend(std::make_unique<mock_memory_allocator_backend>()) == true );
    REQUIRE( manager.register_backend(std::make_unique<mock_memory_allocator_backend>()) == true );
}

TEST_CASE( "memory_allocator_manager does not allow registering null backend", "[memory_allocator_backend]" )
{
    memory_allocator_manager manager;

    REQUIRE( manager.register_backend( nullptr ) == false );
}

TEST_CASE( "creating a allocator from a default initialized memory_allocator_manager should throw", "[memory_allocator_backend]" )
{
    memory_allocator_manager manager;

    mock_memory_resource resource;

    REQUIRE_THROWS_MATCHES(
        manager.create_memory_allocator(resource),
		xmipp4::invalid_operation_error,
		Catch::Matchers::Message(
            "No backends were registered."
        )
	);
}

TEST_CASE( "memory_allocator_manager should use the most suitable backend", "[memory_allocator_backend]" )
{
    memory_allocator_manager manager;

    mock_memory_resource resource;
    auto allocator = std::make_shared<mock_memory_allocator>();

    auto backend1 = std::make_unique<mock_memory_allocator_backend>();
    auto *backend1_ref = backend1.get();
    auto backend2 = std::make_unique<mock_memory_allocator_backend>();
    auto *backend2_ref = backend2.get();
    manager.register_backend(std::move(backend1));
    manager.register_backend(std::move(backend2));

    trompeloeil::sequence seq;

    REQUIRE_CALL(*backend1_ref, get_suitability(ANY(const memory_resource&)))
        .LR_WITH(&_1 == &resource)
        .IN_SEQUENCE(seq)
        .RETURN(xmipp4::backend_priority::fallback);
    REQUIRE_CALL(*backend2_ref, get_suitability(ANY(const memory_resource&)))
        .LR_WITH(&_1 == &resource)
        .IN_SEQUENCE(seq)
        .RETURN(xmipp4::backend_priority::normal);
    REQUIRE_CALL(*backend2_ref, create_memory_allocator(ANY(memory_resource&)))
        .LR_WITH(&_1 == &resource)
        .IN_SEQUENCE(seq)
        .RETURN(allocator);

    REQUIRE( manager.create_memory_allocator(resource) == allocator );
}

TEST_CASE( "memory_allocator_manager should throw when there is no supported backend", "[memory_allocator_backend]" )
{
    memory_allocator_manager manager;

    mock_memory_resource resource;
    auto allocator = std::make_shared<mock_memory_allocator>();

    auto backend1 = std::make_unique<mock_memory_allocator_backend>();
    auto backend2 = std::make_unique<mock_memory_allocator_backend>();

    REQUIRE_CALL(*backend1, get_suitability(ANY(const memory_resource&)))
        .LR_WITH(&_1 == &resource)
        .RETURN(xmipp4::backend_priority::unsupported);
    REQUIRE_CALL(*backend2, get_suitability(ANY(const memory_resource&)))
        .LR_WITH(&_1 == &resource)
        .RETURN(xmipp4::backend_priority::unsupported);

    manager.register_backend(std::move(backend1));
    manager.register_backend(std::move(backend2));

    REQUIRE_THROWS_MATCHES(
        manager.create_memory_allocator(resource),
		xmipp4::invalid_operation_error,
		Catch::Matchers::Message(
            "No backend supports creating allocators for the requested "
            "memory_resource"
        )
	);
}
