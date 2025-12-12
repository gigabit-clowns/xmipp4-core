// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/hardware/device_context.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/memory_allocator_manager.hpp>
#include <xmipp4/core/service_catalog.hpp>

#include "mock/mock_device.hpp"
#include "mock/mock_device_queue.hpp"
#include "mock/mock_device_backend.hpp"
#include "mock/mock_memory_resource.hpp"
#include "mock/mock_memory_allocator.hpp"
#include "mock/mock_memory_allocator_backend.hpp"


using namespace xmipp4;
using namespace xmipp4::hardware;

static
device_context make_test_device_context()
{
	const device_index index("mock", 1234);

	auto device_backend = std::make_unique<mock_device_backend>();
	auto device = std::make_shared<mock_device>();
	mock_memory_resource resource;
	auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();
	auto allocator = std::make_shared<mock_memory_allocator>();

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN("mock");
	REQUIRE_CALL(*device_backend, create_device(index.get_device_id()))
		.RETURN(device);
	
	REQUIRE_CALL(*device, get_device_local_memory_resource())
		.LR_RETURN(resource);
	REQUIRE_CALL(*device, get_host_accessible_memory_resource())
		.LR_RETURN(resource);

	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &resource)
		.RETURN(allocator);

	service_catalog catalog(false);
	catalog.get_service_manager<device_manager>().register_backend(std::move(device_backend));
	catalog.get_service_manager<memory_allocator_manager>().register_backend(std::move(allocator_backend));

	return device_context(catalog, index);
}


TEST_CASE( "Constructing a device_context from an index should create and store the appropiate device resources", "[device_context]" )
{
	const device_index index("mock", 1234);

	auto device_backend = std::make_unique<mock_device_backend>();
	auto device = std::make_shared<mock_device>();
	mock_memory_resource device_local_memory_resource;
	mock_memory_resource host_accessible_memory_resource;
	auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();
	auto device_local_memory_allocator = std::make_shared<mock_memory_allocator>();
	auto host_accessible_memory_allocator = std::make_shared<mock_memory_allocator>();

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN("mock");
	REQUIRE_CALL(*device_backend, create_device(index.get_device_id()))
		.RETURN(device);
	
	REQUIRE_CALL(*device, get_device_local_memory_resource())
		.LR_RETURN(device_local_memory_resource);
	REQUIRE_CALL(*device, get_host_accessible_memory_resource())
		.LR_RETURN(host_accessible_memory_resource);

	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &device_local_memory_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &host_accessible_memory_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &device_local_memory_resource)
		.RETURN(device_local_memory_allocator);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &host_accessible_memory_resource)
		.RETURN(host_accessible_memory_allocator);

	service_catalog catalog(false);
	catalog.get_service_manager<device_manager>().register_backend(std::move(device_backend));
	catalog.get_service_manager<memory_allocator_manager>().register_backend(std::move(allocator_backend));

	device_context context(catalog, index);

	CHECK( &context.get_device() == device.get() );
	CHECK( &context.get_memory_allocator(target_placement::device_optimal) == device_local_memory_allocator.get() );
	CHECK( &context.get_memory_allocator(target_placement::host_accessible) == host_accessible_memory_allocator.get() );
	CHECK( context.get_active_queue() == nullptr );
}

TEST_CASE( "Constructing a device_context for a device with unified memory should share a single allocator", "[device_context]" )
{
	const device_index index("mock", 1234);

	auto device_backend = std::make_unique<mock_device_backend>();
	auto device = std::make_shared<mock_device>();
	mock_memory_resource resource;
	auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();
	auto allocator = std::make_shared<mock_memory_allocator>();

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN("mock");
	REQUIRE_CALL(*device_backend, create_device(index.get_device_id()))
		.RETURN(device);
	
	REQUIRE_CALL(*device, get_device_local_memory_resource())
		.LR_RETURN(resource);
	REQUIRE_CALL(*device, get_host_accessible_memory_resource())
		.LR_RETURN(resource);

	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &resource)
		.RETURN(allocator);

	service_catalog catalog(false);
	catalog.get_service_manager<device_manager>().register_backend(std::move(device_backend));
	catalog.get_service_manager<memory_allocator_manager>().register_backend(std::move(allocator_backend));

	device_context context(catalog, index);

	CHECK( &context.get_device() == device.get() );
	CHECK( &context.get_memory_allocator(target_placement::device_optimal) == allocator.get() );
	CHECK( &context.get_memory_allocator(target_placement::host_accessible) == allocator.get() );
	CHECK( context.get_active_queue() == nullptr );
}

TEST_CASE( "Calling set_active queue on a device_context should update the active queue and return the previous one", "[device_context]" )
{
	auto queue1 = std::make_shared<mock_device_queue>();
	auto queue2 = std::make_shared<mock_device_queue>();
	auto queue3 = std::make_shared<mock_device_queue>();

	auto context = make_test_device_context();

	REQUIRE( context.set_active_queue(queue1) == nullptr );
	CHECK( context.get_active_queue() == queue1 );
	REQUIRE( context.set_active_queue(queue2) == queue1 );
	CHECK( context.get_active_queue() == queue2 );
	REQUIRE( context.set_active_queue(queue3) == queue2 );
	CHECK( context.get_active_queue() == queue3 );
	REQUIRE( context.set_active_queue(nullptr) == queue3 );
	CHECK( context.get_active_queue() == nullptr );
	REQUIRE( context.set_active_queue(queue2) == nullptr );
	CHECK( context.get_active_queue() == queue2 );
}

TEST_CASE( "Calling get_device on a default initialized device_context should throw", "[device_context]" )
{
	device_context context;

	REQUIRE_THROWS_MATCHES(
		context.get_device(),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Can not use a moved or default constructed device_context"
		)
	);
}

TEST_CASE( "Calling get_memory_allocator on a default initialized device_context should throw", "[device_context]" )
{
	device_context context;
	const auto target = GENERATE(
		target_placement::device_optimal, 
		target_placement::host_accessible
	);

	REQUIRE_THROWS_MATCHES(
		context.get_memory_allocator(target),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Can not use a moved or default constructed device_context"
		)
	);
}

TEST_CASE( "Calling get_active_queue on a default initialized device_context should throw", "[device_context]" )
{
	device_context context;
	REQUIRE_THROWS_MATCHES(
		context.get_active_queue(),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Can not use a moved or default constructed device_context"
		)
	);
}

TEST_CASE( "Calling set_active_queue on a default initialized device_context should throw", "[device_context]" )
{
	device_context context;
	REQUIRE_THROWS_MATCHES(
		context.set_active_queue(nullptr),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Can not use a moved or default constructed device_context"
		)
	);
}
