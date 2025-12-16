// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <multidimensional/eager_operation_dispatcher.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/memory_allocator_manager.hpp>

#include "mock/mock_kernel.hpp"
#include "mock/mock_kernel_builder.hpp"
#include "mock/mock_operation.hpp"
#include "../hardware/mock/mock_device.hpp"
#include "../hardware/mock/mock_device_queue.hpp"
#include "../hardware/mock/mock_device_backend.hpp"
#include "../hardware/mock/mock_memory_resource.hpp"
#include "../hardware/mock/mock_memory_allocator.hpp"
#include "../hardware/mock/mock_memory_allocator_backend.hpp"

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
	mock_memory_resource resource;
	auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();
	auto allocator = std::make_shared<mock_memory_allocator>();

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN("mock");
	REQUIRE_CALL(*device_backend, get_device_properties(index.get_device_id(), ANY(device_properties&)))
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
	catalog.get_service_manager<device_manager>()
		.register_backend(std::move(device_backend));
	catalog.get_service_manager<memory_allocator_manager>()
		.register_backend(std::move(allocator_backend));

	return execution_context(catalog, index);
}

// TODO
