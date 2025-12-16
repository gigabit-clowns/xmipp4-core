// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <hardware/memory_allocator_pool.hpp>

#include <xmipp4/core/hardware/memory_allocator_manager.hpp>

#include "mock/mock_device.hpp"
#include "mock/mock_memory_resource.hpp"
#include "mock/mock_memory_allocator.hpp"
#include "mock/mock_memory_allocator_backend.hpp"

using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE( "Constructing a memory_allocator_pool with a distributed memory device should create two allocators", "[device_allocator_pool]" )
{

	mock_device device;

	mock_memory_resource device_optimal_memory_resource;
	mock_memory_resource host_accessible_memory_resource;
	auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();
	auto device_optimal_memory_allocator = std::make_shared<mock_memory_allocator>();
	auto host_accessible_memory_allocator = std::make_shared<mock_memory_allocator>();

	REQUIRE_CALL(device, get_memory_resource(target_placement::device_optimal))
		.LR_RETURN(device_optimal_memory_resource);
	REQUIRE_CALL(device, get_memory_resource(target_placement::host_accessible))
		.LR_RETURN(host_accessible_memory_resource);

	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &device_optimal_memory_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &host_accessible_memory_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &device_optimal_memory_resource)
		.RETURN(device_optimal_memory_allocator);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &host_accessible_memory_resource)
		.RETURN(host_accessible_memory_allocator);

	memory_allocator_manager allocator_manager;
	REQUIRE( allocator_manager.register_backend(std::move(allocator_backend)) );

	memory_allocator_pool pool(device, allocator_manager);
	CHECK( 
		&pool.get_memory_allocator(target_placement::device_optimal) == 
		device_optimal_memory_allocator.get() 
	);
	CHECK( 
		&pool.get_memory_allocator(target_placement::host_accessible) == 
		host_accessible_memory_allocator.get() 
	);
}

TEST_CASE( "Constructing a memory_allocator_pool with an unified device should create a single allocator", "[device_allocator_pool]" )
{

	mock_device device;

	mock_memory_resource resource;
	auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();
	auto allocator = std::make_shared<mock_memory_allocator>();

	REQUIRE_CALL(device, get_memory_resource(target_placement::device_optimal))
		.LR_RETURN(resource);
	REQUIRE_CALL(device, get_memory_resource(target_placement::host_accessible))
		.LR_RETURN(resource);

	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &resource)
		.RETURN(allocator);

	memory_allocator_manager allocator_manager;
	REQUIRE( allocator_manager.register_backend(std::move(allocator_backend)) );

	memory_allocator_pool pool(device, allocator_manager);
	CHECK( 
		&pool.get_memory_allocator(target_placement::device_optimal) == 
		allocator.get() 
	);
	CHECK( 
		&pool.get_memory_allocator(target_placement::host_accessible) == 
		allocator.get() 
	);
}
