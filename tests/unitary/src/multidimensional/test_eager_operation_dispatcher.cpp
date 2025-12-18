// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <multidimensional/eager_operation_dispatcher.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
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
	mock_operation operation;
	auto kernel_builder = std::make_unique<mock_kernel_builder>();
	auto kernel = std::make_shared<mock_kernel>();

	execution_context context;
	{
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

		context = execution_context(catalog, index);
	}

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

	std::array<std::shared_ptr<buffer>, 3> input_buffers;
	std::array<array_view, 3> input_arrays;
	for (std::size_t i = 0; i < input_arrays.size(); ++i)
	{

		input_buffers[i] = std::make_shared<buffer>(
			nullptr, 
			24576, 
			device_resource, 
			nullptr
		);

		REQUIRE_CALL(*device_allocator, get_max_alignment())
			.RETURN(256);
		REQUIRE_CALL(*device_allocator, allocate(24576, 256, nullptr))
			.RETURN(input_buffers[i]);

		input_arrays[i] = empty(
			descriptor1, 
			memory_resource_affinity::device,
			context
		);
	}

	std::array<std::shared_ptr<buffer>, 2> output_buffers;
	std::array<array, 2> output_arrays;
	for (std::size_t i = 0; i < output_arrays.size(); ++i)
	{
		output_buffers[i] = std::make_shared<buffer>(
			nullptr, 
			24576, 
			device_resource, 
			nullptr
		);

		REQUIRE_CALL(*device_allocator, get_max_alignment())
			.RETURN(256);
		REQUIRE_CALL(*device_allocator, allocate(24576, 256, nullptr))
			.RETURN(output_buffers[i]);

		output_arrays[i] = empty(
			descriptor1, 
			memory_resource_affinity::device,
			context
		);
	}

	ALLOW_CALL(*device_allocator, get_memory_resource())
		.LR_RETURN(device_resource);
	ALLOW_CALL(*device, get_memory_resource(memory_resource_affinity::device))
		.LR_RETURN(device_resource);

	REQUIRE_CALL(*kernel_builder, get_suitability(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.LR_WITH(&_1 == &operation)
		.WITH(_2.size() == 5)
		.WITH(_2[0] == descriptor1)
		.WITH(_2[1] == descriptor1)
		.WITH(_2[2] == descriptor1)
		.WITH(_2[3] == descriptor1)
		.WITH(_2[4] == descriptor1)
		.LR_WITH(&_3 == &context.get_device())
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*kernel_builder, build(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.LR_WITH(&_1 == &operation)
		.WITH(_2.size() == 5)
		.WITH(_2[0] == descriptor1)
		.WITH(_2[1] == descriptor1)
		.WITH(_2[2] == descriptor1)
		.WITH(_2[3] == descriptor1)
		.WITH(_2[4] == descriptor1)
		.LR_WITH(&_3 == &context.get_device())
		.RETURN(kernel);

	{
		REQUIRE_CALL(*kernel_builder, get_operation_id())
			.LR_RETURN(operation.get_id());

		catalog.get_service_manager<kernel_manager>()
			.register_kernel(std::move(kernel_builder));
	}

	REQUIRE_CALL(operation, sanitize_operands(trompeloeil::_, trompeloeil::_))
		.WITH(_1.size() == 2)
		.WITH(_1[0] == descriptor1)
		.WITH(_1[1] == descriptor1)
		.WITH(_2.size() == 3)
		.WITH(_2[0] == descriptor1)
		.WITH(_2[1] == descriptor1)
		.WITH(_2[2] == descriptor1);

	REQUIRE_CALL(*kernel, execute(trompeloeil::_, trompeloeil::_, nullptr))
		.WITH(_1.size() == 2)
		.WITH(_1[0] == output_buffers[0])
		.WITH(_1[1] == output_buffers[1])
		.WITH(_2.size() == 3)
		.WITH(_2[0] == input_buffers[0])
		.WITH(_2[1] == input_buffers[1])
		.WITH(_2[2] == input_buffers[2]);

	eager_operation_dispatcher dispatcher(
		catalog.get_service_manager<kernel_manager>()
	);
	dispatcher.dispatch(
		operation,
		make_span(output_arrays),
		make_span(input_arrays),
		context
	);
}

/*
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
