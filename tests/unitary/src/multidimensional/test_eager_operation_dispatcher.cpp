// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <multidimensional/eager_operation_dispatcher.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/array_factory.hpp>
#include <xmipp4/core/multidimensional/kernel_manager.hpp>
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

/*
TEST_CASE("eager_operation_dispatcher should execute a properly configured kernel", "[eager_operation_dispatcher]")
{
	const hardware::device_index index("mock", 1234);
	auto device_backend = std::make_unique<mock_device_backend>();
	auto device = std::make_shared<mock_device>();
	mock_memory_resource device_resource;
	mock_memory_resource host_resource;
	auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();
	auto device_allocator = std::make_shared<mock_memory_allocator>();
	auto host_allocator = std::make_shared<mock_memory_allocator>();
	service_catalog catalog(false);
	kernel_manager kernels;
	mock_operation operation;

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
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &device_resource)
		.RETURN(device_allocator);
	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const memory_resource&)))
		.LR_WITH(&_1 == &host_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(memory_resource&)))
		.LR_WITH(&_1 == &host_resource)
		.RETURN(host_allocator);

	catalog.get_service_manager<device_manager>()
		.register_backend(std::move(device_backend));
	catalog.get_service_manager<memory_allocator_manager>()
		.register_backend(std::move(allocator_backend));

	execution_context context(catalog, index);

	const std::array<std::size_t, 4> extents1 = {8, 64, 3, 4};
	const array_descriptor descriptor1(
		strided_layout::make_contiguous_layout(make_span(extents1)),
		numerical_type::float32
	);
	const std::array<std::size_t, 2> extents2 = {8, 64};
	const array_descriptor descriptor2(
		strided_layout::make_contiguous_layout(make_span(extents1)),
		numerical_type::int64
	);

	std::array<std::shared_ptr<buffer>, 5> buffers;
	for (std::size_t i = 0; i < buffers.size(); ++i)
	{
		buffers[i] = std::make_shared<buffer>(
			nullptr, 
			24576, 
			device_resource, 
			nullptr
		);
	}

	REQUIRE_CALL(*device_allocator, get_max_alignment())
		.TIMES(5)
		.RETURN(256);
	REQUIRE_CALL(*device_allocator, allocate(24576, 256, nullptr))
		.RETURN(buffers[0]);
	REQUIRE_CALL(*device_allocator, allocate(4096, 256, nullptr))
		.RETURN(buffers[1]);
	REQUIRE_CALL(*device_allocator, allocate(24576, 256, nullptr))
		.RETURN(buffers[2]);
	REQUIRE_CALL(*device_allocator, allocate(4096, 256, nullptr))
		.RETURN(buffers[3]);
	REQUIRE_CALL(*device_allocator, allocate(4096, 256, nullptr))
		.RETURN(buffers[4]);

	std::array<const array, 3> input_arrays = {
		empty(descriptor1, memory_resource_affinity::device, context),
		empty(descriptor2, memory_resource_affinity::device, context),
		empty(descriptor1, memory_resource_affinity::device, context),
	};
	std::array<array, 2> output_arrays;

	SECTION( "pre-allocated output" )
	{
		for (auto &out : output_arrays)
		{
			out = empty(descriptor2, memory_resource_affinity::device, context);
		}
	}

	SECTION( "empty output" )
	{
	}

	eager_operation_dispatcher dispatcher(kernels);
	dispatcher.dispatch(
		operation,
		make_span(output_arrays),
		make_span(input_arrays),
		context
	);
}

TEST_CASE("eager_operation_dispatcher should not allocate the output when output arrays are provided", "[eager_operation_dispatcher]")
{
	// TODO
}

TEST_CASE("eager_operation_dispatcher should throw when an storageless input is provided", "[eager_operation_dispatcher]")
{
	// TODO
}

TEST_CASE("eager_operation_dispatcher should throw when an inaccessible input is provided", "[eager_operation_dispatcher]")
{
	// TODO
}

TEST_CASE("eager_operation_dispatcher should record queues for all operands when a queue is provided", "[eager_operation_dispatcher]")
{
	// TODO
}
*/
