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

namespace {

template<typename ArrayT>
void allocate_device_arrays(
	std::size_t count,
	const array_descriptor &descriptor,
	memory_resource &device_resource,
	const std::shared_ptr<mock_memory_allocator> &device_allocator,
	execution_context &context,
	std::vector<std::shared_ptr<buffer>> &buffers,
	std::vector<ArrayT> &arrays
)
{
	buffers.resize(count);
	arrays.resize(count);

	const auto size = compute_storage_requirement(descriptor);

	for (std::size_t i = 0; i < count; ++i)
	{
		buffers[i] = std::make_shared<buffer>(
			nullptr, 
			size, 
			device_resource, 
			nullptr
		);

		REQUIRE_CALL(*device_allocator, get_max_alignment())
			.RETURN(256);
		REQUIRE_CALL(*device_allocator, allocate(size, 256, nullptr))
			.RETURN(buffers[i]);

		arrays[i] = empty(
			descriptor,
			memory_resource_affinity::device,
			context
		);
	}
}
} // anonymous namespace

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

	const std::array<std::size_t, 4> extents = {8, 64, 3, 4};
	const array_descriptor descriptor(
		strided_layout::make_contiguous_layout(make_span(extents)),
		numerical_type::float32
	);

	std::vector<std::shared_ptr<buffer>> input_buffers;
	std::vector<array_view> input_arrays;
	allocate_device_arrays<array_view>(
		3, 
		descriptor, 
		device_resource, 
		device_allocator, 
		context,
		input_buffers, 
		input_arrays
	);

	std::vector<std::shared_ptr<buffer>> output_buffers;
	std::vector<array> output_arrays;
	allocate_device_arrays<array>(
		2, 
		descriptor, 
		device_resource, 
		device_allocator, 
		context,
		output_buffers, 
		output_arrays
	);

	ALLOW_CALL(*device_allocator, get_memory_resource())
		.LR_RETURN(device_resource);
	ALLOW_CALL(*device, get_memory_resource(memory_resource_affinity::device))
		.LR_RETURN(device_resource);

	REQUIRE_CALL(*kernel_builder, get_suitability(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.LR_WITH(&_1 == &operation)
		.WITH(_2.size() == 5)
		.WITH(_2[0] == descriptor)
		.WITH(_2[1] == descriptor)
		.WITH(_2[2] == descriptor)
		.WITH(_2[3] == descriptor)
		.WITH(_2[4] == descriptor)
		.LR_WITH(&_3 == &context.get_device())
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*kernel_builder, build(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.LR_WITH(&_1 == &operation)
		.WITH(_2.size() == 5)
		.WITH(_2[0] == descriptor)
		.WITH(_2[1] == descriptor)
		.WITH(_2[2] == descriptor)
		.WITH(_2[3] == descriptor)
		.WITH(_2[4] == descriptor)
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
		.WITH(_1[0] == descriptor)
		.WITH(_1[1] == descriptor)
		.WITH(_2.size() == 3)
		.WITH(_2[0] == descriptor)
		.WITH(_2[1] == descriptor)
		.WITH(_2[2] == descriptor);

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
