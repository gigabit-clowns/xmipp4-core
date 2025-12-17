// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/execution_context.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/memory_allocator_manager.hpp>
#include <xmipp4/core/service_catalog.hpp>
#include <multidimensional/eager_operation_dispatcher.hpp>

#include "hardware/mock/mock_device.hpp"
#include "hardware/mock/mock_device_queue.hpp"
#include "hardware/mock/mock_device_backend.hpp"
#include "hardware/mock/mock_memory_resource.hpp"
#include "hardware/mock/mock_memory_allocator.hpp"
#include "hardware/mock/mock_memory_allocator_backend.hpp"

#include <stdexcept>


using namespace xmipp4;
using namespace xmipp4::hardware;
using namespace xmipp4::multidimensional;

static
execution_context make_test_execution_context()
{
	const hardware::device_index index("mock", 1234);

	auto device_backend = std::make_unique<mock_device_backend>();
	auto device = std::make_shared<mock_device>();
	mock_memory_resource device_resource;
	mock_memory_resource host_resource;
	const auto device_allocator = 
		std::make_shared<mock_memory_allocator>();
	const auto host_allocator = 
		std::make_shared<mock_memory_allocator>();
	auto allocator_backend = 
		std::make_unique<mock_memory_allocator_backend>();

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN(index.get_backend_name());
	REQUIRE_CALL(*device_backend, get_device_properties(index.get_device_id(), ANY(device_properties&)))
		.LR_SIDE_EFFECT(_2.set_optimal_data_alignment(256))
		.RETURN(true);
	REQUIRE_CALL(*device_backend, create_device(index.get_device_id()))
		.RETURN(device);
	
	REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::device))
		.LR_RETURN(device_resource);
	REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::host))
		.LR_RETURN(host_resource);

	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &device_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &host_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &device_resource)
		.RETURN(device_allocator);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &host_resource)
		.RETURN(host_allocator);

	service_catalog catalog(false);
	catalog.get_service_manager<device_manager>()
		.register_backend(std::move(device_backend));
	catalog.get_service_manager<memory_allocator_manager>()
		.register_backend(std::move(allocator_backend));

	return execution_context(catalog, index);
}



TEST_CASE( "Constructing a execution_context from an index should create and store the appropiate device resources", "[execution_context]" )
{
	const device_index index("mock", 1234);

	auto device_backend = std::make_unique<mock_device_backend>();
	auto device = std::make_shared<mock_device>();
	mock_memory_resource device_resource;
	mock_memory_resource host_resource;
	auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();
	auto device_allocator = std::make_shared<mock_memory_allocator>();
	auto host_allocator = std::make_shared<mock_memory_allocator>();
	device_properties properties;
	properties.set_name("GPU-bought-to-a-cryptobro");

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN("mock");
	REQUIRE_CALL(*device_backend, get_device_properties(index.get_device_id(), ANY(device_properties&)))
		.LR_SIDE_EFFECT(_2 = properties)
		.RETURN(true);
	REQUIRE_CALL(*device_backend, create_device(index.get_device_id()))
		.RETURN(device);
	
	REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::device))
		.LR_RETURN(device_resource);
	REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::host))
		.LR_RETURN(host_resource);

	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &device_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const hardware::memory_resource&)))
		.LR_WITH(&_1 == &host_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &device_resource)
		.RETURN(device_allocator);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &host_resource)
		.RETURN(host_allocator);

	service_catalog catalog(false);
	catalog.get_service_manager<device_manager>()
		.register_backend(std::move(device_backend));
	catalog.get_service_manager<memory_allocator_manager>()
		.register_backend(std::move(allocator_backend));

	execution_context context(catalog, index);

	CHECK( &context.get_device() == device.get() );
	CHECK( &context.get_memory_allocator(memory_resource_affinity::device) == device_allocator.get() );
	CHECK( &context.get_memory_allocator(memory_resource_affinity::host) == host_allocator.get() );
	CHECK( context.get_active_queue() == nullptr );
	CHECK( context.get_device_properties().get_name() == properties.get_name() );
	CHECK( dynamic_cast<eager_operation_dispatcher*>(&context.get_operation_dispatcher()) );
}

TEST_CASE( "Constructing a execution_context for a device with unified memory should share a single allocator", "[execution_context]" )
{
	const device_index index("mock", 1234);

	auto device_backend = std::make_unique<mock_device_backend>();
	auto device = std::make_shared<mock_device>();
	mock_memory_resource resource;
	auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();
	auto allocator = std::make_shared<mock_memory_allocator>();
	device_properties properties;
	properties.set_name("Z80");

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN("mock");
	REQUIRE_CALL(*device_backend, get_device_properties(index.get_device_id(), ANY(device_properties&)))
		.LR_SIDE_EFFECT(_2 = properties)
		.RETURN(true);
	REQUIRE_CALL(*device_backend, create_device(index.get_device_id()))
		.RETURN(device);
	
	REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::device))
		.LR_RETURN(resource);
	REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::host))
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

	execution_context context(catalog, index);

	CHECK( &context.get_device() == device.get() );
	CHECK( &context.get_memory_allocator(memory_resource_affinity::device) == allocator.get() );
	CHECK( &context.get_memory_allocator(memory_resource_affinity::host) == allocator.get() );
	CHECK( context.get_active_queue() == nullptr );
	CHECK( context.get_device_properties().get_name() == properties.get_name() );
	CHECK( dynamic_cast<eager_operation_dispatcher*>(&context.get_operation_dispatcher()) );
}

TEST_CASE( "Calling set_active queue on a execution_context should update the active queue and return the previous one", "[execution_context]" )
{
	auto queue1 = std::make_shared<mock_device_queue>();
	auto queue2 = std::make_shared<mock_device_queue>();
	auto queue3 = std::make_shared<mock_device_queue>();

	auto context = make_test_execution_context();

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

TEST_CASE( "Constructing a execution_context from an invalid index should throw", "[execution_context]" )
{
	const device_index index("mock", 1234);

	auto device_backend = std::make_unique<mock_device_backend>();

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN("mock");
	REQUIRE_CALL(*device_backend, get_device_properties(index.get_device_id(), ANY(device_properties&)))
		.RETURN(false);
	
	service_catalog catalog(false);
	catalog.get_service_manager<device_manager>()
		.register_backend(std::move(device_backend));

	REQUIRE_THROWS_MATCHES(
		execution_context(catalog, index),
		std::invalid_argument,
		Catch::Matchers::Message("Requested device index does not exist")
	);
}

TEST_CASE( "Calling get_device on a default initialized execution_context should throw", "[execution_context]" )
{
	execution_context context;

	REQUIRE_THROWS_MATCHES(
		context.get_device(),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Can not use a moved or default constructed execution_context"
		)
	);
}

TEST_CASE( "Calling get_memory_allocator on a default initialized execution_context should throw", "[execution_context]" )
{
	execution_context context;
	const auto target = GENERATE(
		memory_resource_affinity::device, 
		memory_resource_affinity::host
	);

	REQUIRE_THROWS_MATCHES(
		context.get_memory_allocator(target),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Can not use a moved or default constructed execution_context"
		)
	);
}

TEST_CASE( "Calling get_active_queue on a default initialized execution_context should throw", "[execution_context]" )
{
	execution_context context;
	REQUIRE_THROWS_MATCHES(
		context.get_active_queue(),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Can not use a moved or default constructed execution_context"
		)
	);
}

TEST_CASE( "Calling set_active_queue on a default initialized execution_context should throw", "[execution_context]" )
{
	execution_context context;
	REQUIRE_THROWS_MATCHES(
		context.set_active_queue(nullptr),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Can not use a moved or default constructed execution_context"
		)
	);
}
