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
#include "../hardware/mock/mock_buffer_sentinel.hpp"
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

execution_context create_execution_context(
	service_catalog &catalog,
	const std::shared_ptr<mock_device> &device,
	memory_resource &device_resource,
	const std::shared_ptr<mock_memory_allocator> &device_allocator,
	memory_resource &host_resource,
	const std::shared_ptr<mock_memory_allocator> &host_allocator,
	const std::shared_ptr<mock_device_queue> &queue
)
{
	const hardware::device_index index("mock", 1234);

	auto device_backend = std::make_unique<mock_device_backend>();
	auto allocator_backend = std::make_unique<mock_memory_allocator_backend>();

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN(index.get_backend_name());
	REQUIRE_CALL(*device_backend, get_device_properties(index.get_device_id(), trompeloeil::_))
		.LR_SIDE_EFFECT(_2.set_optimal_data_alignment(256))
		.RETURN(true);
	REQUIRE_CALL(*device_backend, create_device(index.get_device_id()))
		.RETURN(device);
	
	REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::device))
		.LR_RETURN(device_resource);
	REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::host))
		.LR_RETURN(host_resource);

	REQUIRE_CALL(*allocator_backend, get_suitability(trompeloeil::_))
		.LR_WITH(&_1 == &device_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(trompeloeil::_))
		.LR_WITH(&_1 == &device_resource)
		.RETURN(device_allocator);
	REQUIRE_CALL(*allocator_backend, get_suitability(trompeloeil::_))
		.LR_WITH(&_1 == &host_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(trompeloeil::_))
		.LR_WITH(&_1 == &host_resource)
		.RETURN(host_allocator);

	catalog.get_service_manager<device_manager>()
		.register_backend(std::move(device_backend));
	catalog.get_service_manager<memory_allocator_manager>()
		.register_backend(std::move(allocator_backend));

	execution_context result(catalog, index);
	result.set_active_queue(queue);

	return result;
}

array_descriptor create_array_descriptor()
{
	const std::array<std::size_t, 4> extents = {8, 64, 3, 4};
	return array_descriptor(
		strided_layout::make_contiguous_layout(make_span(extents)),
		numerical_type::float32
	);
}

std::vector<std::shared_ptr<buffer>> create_buffers(
	std::size_t count,
	const array_descriptor &descriptor,
	memory_resource &device_resource
)
{
	const auto size = compute_storage_requirement(descriptor);

	std::vector<std::shared_ptr<buffer>> result;
	result.reserve(count);

	for (std::size_t i = 0; i < count; ++i)
	{
		auto item = std::make_shared<buffer>(
			nullptr, 
			size, 
			device_resource, 
			std::make_unique<mock_buffer_sentinel>()
		);

		result.push_back(std::move(item));
	}

	return result;
}

template<typename ArrayT>
std::vector<ArrayT> create_device_arrays(
	const std::vector<std::shared_ptr<buffer>> &buffers,
	const array_descriptor &descriptor,
	execution_context &context
)
{
	const auto affinity = memory_resource_affinity::device;
	const auto &queue = context.get_active_queue();
	auto &allocator = static_cast<mock_memory_allocator&>(
		context.get_memory_allocator(affinity)
	);

	std::vector<ArrayT> result;
	result.reserve(buffers.size());


	for (const auto &buf : buffers)
	{
		const auto size = buf->get_size();
		REQUIRE_CALL(allocator, get_max_alignment())
			.RETURN(256);
		REQUIRE_CALL(allocator, allocate(size, 256, queue.get()))
			.RETURN(buf);

		auto item = empty(
			descriptor,
			memory_resource_affinity::device,
			context
		);
		
		result.push_back(std::move(item));
	}

	return result;
}

void require_call_to_record_buffer(
	const std::vector<std::shared_ptr<buffer>> &buffers,
	device_queue &queue,
	std::vector<std::unique_ptr<trompeloeil::expectation>> &expectations
)
{
	for (const auto &buffer : buffers)
	{
		auto *sentinel = 
			static_cast<mock_buffer_sentinel*>(buffer->get_sentinel());
		auto expectation = 
			NAMED_REQUIRE_CALL(*sentinel, record_queue(trompeloeil::_, false))
				.LR_WITH(&_1 == &queue);
		expectations.push_back(std::move(expectation));
	}
}

} // anonymous namespace

TEST_CASE("eager_operation_dispatcher should execute a properly configured kernel", "[eager_operation_dispatcher]")
{
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations;

	service_catalog catalog(false);
	auto device = std::make_shared<mock_device>();
	mock_memory_resource device_resource;
	mock_memory_resource host_resource;
	auto device_allocator = std::make_shared<mock_memory_allocator>();
	auto host_allocator = std::make_shared<mock_memory_allocator>();
	mock_operation operation;
	auto kernel_builder = std::make_unique<mock_kernel_builder>();
	auto kernel = std::make_shared<mock_kernel>();
	auto queue = GENERATE(
		std::shared_ptr<mock_device_queue>(),
		std::make_shared<mock_device_queue>()
	);

	auto context = create_execution_context(
		catalog,
		device,
		device_resource,
		device_allocator,
		host_resource,
		host_allocator,
		queue
	);

	const auto descriptor = create_array_descriptor();

	const auto input_buffers = create_buffers(
		3,
		descriptor,
		device_resource
	);
	const auto input_arrays = create_device_arrays<array_view>(
		input_buffers,
		descriptor, 
		context
	);

	const auto output_buffers = create_buffers(
		2,
		descriptor,
		device_resource
	);

	std::vector<array> output_arrays;
	SECTION("provided a pre-allocated output should re-use")
	{
		output_arrays = create_device_arrays<array>(
			output_buffers, 
			descriptor, 
			context
		);
	}
	SECTION("if empty output is provided it should allocate")
	{
		auto &allocator = *device_allocator;

		output_arrays.resize(2);
		for (const auto &buffer : output_buffers)
		{
			const auto size = buffer->get_size();
			auto expectation1 = NAMED_REQUIRE_CALL(allocator, get_max_alignment())
				.RETURN(256);
			auto expectation2 = NAMED_REQUIRE_CALL(allocator, allocate(size, 256, queue.get()))
				.RETURN(buffer);

			expectations.push_back(std::move(expectation1));
			expectations.push_back(std::move(expectation2));
		}
	}

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
		.LR_WITH(_1[0] == output_arrays[0].get_descriptor())
		.LR_WITH(_1[1] == output_arrays[1].get_descriptor())
		.WITH(_2.size() == 3)
		.WITH(_2[0] == descriptor)
		.WITH(_2[1] == descriptor)
		.WITH(_2[2] == descriptor)
		.SIDE_EFFECT(_1[0] = descriptor)
		.SIDE_EFFECT(_1[1] = descriptor);

	REQUIRE_CALL(*kernel, execute(trompeloeil::_, trompeloeil::_, queue.get()))
		.WITH(_1.size() == 2)
		.WITH(_2.size() == 3)
		.WITH(_2[0] == input_buffers[0])
		.WITH(_2[1] == input_buffers[1])
		.WITH(_2[2] == input_buffers[2]);

	if(queue)
	{
		require_call_to_record_buffer(input_buffers, *queue, expectations);
		require_call_to_record_buffer(output_buffers, *queue, expectations);
	}

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

TEST_CASE("eager_operation_dispatcher should throw if an storage-less input array is provided", "[eager_operation_dispatcher]")
{
	service_catalog catalog(false);
	auto device = std::make_shared<mock_device>();
	mock_memory_resource device_resource;
	mock_memory_resource host_resource;
	auto device_allocator = std::make_shared<mock_memory_allocator>();
	auto host_allocator = std::make_shared<mock_memory_allocator>();
	mock_operation operation;
	auto kernel_builder = std::make_unique<mock_kernel_builder>();
	auto kernel = std::make_shared<mock_kernel>();
	auto queue = GENERATE(
		std::shared_ptr<mock_device_queue>(),
		std::make_shared<mock_device_queue>()
	);

	auto context = create_execution_context(
		catalog,
		device,
		device_resource,
		device_allocator,
		host_resource,
		host_allocator,
		queue
	);

	const auto descriptor = create_array_descriptor();

	const auto input_buffers = create_buffers(
		2,
		descriptor,
		device_resource
	);
	auto input_arrays = create_device_arrays<array_view>(
		input_buffers,
		descriptor, 
		context
	);
	array empty(nullptr, descriptor);
	input_arrays.emplace_back(empty);


	const auto output_buffers = create_buffers(
		2,
		descriptor,
		device_resource
	);

	auto output_arrays = create_device_arrays<array>(
		output_buffers, 
		descriptor, 
		context
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

	eager_operation_dispatcher dispatcher(
		catalog.get_service_manager<kernel_manager>()
	);

	REQUIRE_THROWS_MATCHES(
		dispatcher.dispatch(
			operation,
			make_span(output_arrays),
			make_span(input_arrays),
			context
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"One of the input operands does not an associated storage"
		)
	);
}

TEST_CASE("eager_operation_dispatcher should throw if an input with an inappropriate memory resource is provided", "[eager_operation_dispatcher]")
{
	service_catalog catalog(false);
	auto device = std::make_shared<mock_device>();
	mock_memory_resource device_resource;
	mock_memory_resource host_resource;
	auto device_allocator = std::make_shared<mock_memory_allocator>();
	auto host_allocator = std::make_shared<mock_memory_allocator>();
	mock_operation operation;
	auto kernel_builder = std::make_unique<mock_kernel_builder>();
	auto kernel = std::make_shared<mock_kernel>();
	auto queue = GENERATE(
		std::shared_ptr<mock_device_queue>(),
		std::make_shared<mock_device_queue>()
	);

	auto context = create_execution_context(
		catalog,
		device,
		device_resource,
		device_allocator,
		host_resource,
		host_allocator,
		queue
	);

	const auto descriptor = create_array_descriptor();

	const auto input_buffers = create_buffers(
		2,
		descriptor,
		device_resource
	);
	auto input_arrays = create_device_arrays<array_view>(
		input_buffers,
		descriptor, 
		context
	);
	mock_memory_resource inappropriate_resource;
	auto inappropriate_buffer = std::make_shared<buffer>(
		nullptr, 
		1024, 
		inappropriate_resource, 
		nullptr
	);
	array inappropiate_array(inappropriate_buffer, descriptor);
	input_arrays.emplace_back(inappropiate_array);


	const auto output_buffers = create_buffers(
		2,
		descriptor,
		device_resource
	);

	auto output_arrays = create_device_arrays<array>(
		output_buffers, 
		descriptor, 
		context
	);

	ALLOW_CALL(*device_allocator, get_memory_resource())
		.LR_RETURN(device_resource);
	ALLOW_CALL(*device, get_memory_resource(memory_resource_affinity::device))
		.LR_RETURN(device_resource);

	REQUIRE_CALL(inappropriate_resource, get_target_device())
		.RETURN(nullptr);

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

	eager_operation_dispatcher dispatcher(
		catalog.get_service_manager<kernel_manager>()
	);

	REQUIRE_THROWS_MATCHES(
		dispatcher.dispatch(
			operation,
			make_span(output_arrays),
			make_span(input_arrays),
			context
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"One of the input operands is not accessible by the device "
			"used to execute the operation"
		)
	);
}
