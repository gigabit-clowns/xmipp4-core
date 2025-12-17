// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/array_factory.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_index.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/memory_allocator_manager.hpp>
#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/service_catalog.hpp>

#include <hardware/host_memory/host_buffer.hpp>

#include "../hardware/mock/mock_device.hpp"
#include "../hardware/mock/mock_device_queue.hpp"
#include "../hardware/mock/mock_device_backend.hpp"
#include "../hardware/mock/mock_memory_resource.hpp"
#include "../hardware/mock/mock_memory_allocator.hpp"
#include "../hardware/mock/mock_memory_allocator_backend.hpp"

using namespace xmipp4;
using namespace xmipp4::multidimensional;


execution_context make_test_device_context()
{
	const hardware::device_index index("mock", 1234);

	auto device_backend = std::make_unique<hardware::mock_device_backend>();
	auto device = std::make_shared<hardware::mock_device>();
	hardware::mock_memory_resource device_memory_resource;
	hardware::mock_memory_resource host_memory_resource;
	const auto device_allocator = 
		std::make_shared<hardware::mock_memory_allocator>();
	const auto host_allocator = 
		std::make_shared<hardware::mock_memory_allocator>();
	auto allocator_backend = 
		std::make_unique<hardware::mock_memory_allocator_backend>();

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN(index.get_backend_name());
	REQUIRE_CALL(*device_backend, get_device_properties(index.get_device_id(), ANY(hardware::device_properties&)))
		.LR_SIDE_EFFECT(_2.set_optimal_data_alignment(256))
		.RETURN(true);
	REQUIRE_CALL(*device_backend, create_device(index.get_device_id()))
		.RETURN(device);
	
	REQUIRE_CALL(*device, get_memory_resource(hardware::memory_resource_affinity::device))
		.LR_RETURN(device_memory_resource);
	REQUIRE_CALL(*device, get_memory_resource(hardware::memory_resource_affinity::host))
		.LR_RETURN(host_memory_resource);

	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const hardware::memory_resource&)))
		.LR_WITH(&_1 == &device_memory_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const hardware::memory_resource&)))
		.LR_WITH(&_1 == &host_memory_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(hardware::memory_resource&)))
		.LR_WITH(&_1 == &device_memory_resource)
		.RETURN(device_allocator);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(hardware::memory_resource&)))
		.LR_WITH(&_1 == &host_memory_resource)
		.RETURN(host_allocator);

	service_catalog catalog(false);
	catalog.get_service_manager<hardware::device_manager>()
		.register_backend(std::move(device_backend));
	catalog.get_service_manager<hardware::memory_allocator_manager>()
		.register_backend(std::move(allocator_backend));

	return execution_context(catalog, index);
}

TEST_CASE("Calling empty without an output array should allocate with the appropiate allocator.", "[array_factory]")
{
	const auto context = make_test_device_context();

	const std::array<std::size_t, 3> extents = {10, 256, 129};
	const std::array<std::ptrdiff_t, 3> strides = {198144, 774, 2};
	const std::ptrdiff_t offset = 10;

	const auto data_type = GENERATE(
		numerical_type::int16, 
		numerical_type::float32
	);

	const array_descriptor descriptor(
		strided_layout::make_custom_layout(
			make_span(extents),
			make_span(strides),
			offset
		),
		data_type
	);

	const auto alloc_bytes = compute_storage_requirement(descriptor);
	const auto buffer = 
		std::make_shared<hardware::host_buffer>(alloc_bytes, 256);

	const auto affinity = GENERATE(
		hardware::memory_resource_affinity::device,
		hardware::memory_resource_affinity::host
	);

	auto &allocator = dynamic_cast<hardware::mock_memory_allocator&>(
		context.get_memory_allocator(affinity)
	);

	REQUIRE_CALL(allocator, get_max_alignment())
		.RETURN(256);
	REQUIRE_CALL(allocator, allocate(alloc_bytes, 256, nullptr))
		.RETURN(buffer);

	const auto result = empty(descriptor, affinity, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	CHECK( result.get_storage() == buffer.get() );
}

TEST_CASE("Calling empty with an output array with the same descriptor should return an alias", "[array_factory]")
{
	const auto context = make_test_device_context();

	const std::array<std::size_t, 3> extents = {80, 16, 256};

	const array_descriptor descriptor(
		strided_layout::make_contiguous_layout(make_span(extents)),
		numerical_type::float32
	);

	const auto alloc_bytes = compute_storage_requirement(descriptor);
	const auto storage = std::make_shared<hardware::host_buffer>(alloc_bytes, 256);

	const auto affinity = GENERATE(
		hardware::memory_resource_affinity::device,
		hardware::memory_resource_affinity::host
	);

	auto &allocator = dynamic_cast<hardware::mock_memory_allocator&>(
		context.get_memory_allocator(affinity)
	);
	REQUIRE_CALL(allocator, get_memory_resource())
		.LR_RETURN(hardware::get_host_memory_resource());

	array array1(storage, descriptor);
	const auto array2 = empty(descriptor, affinity, context, &array1);

	CHECK( array1.get_descriptor() == descriptor );
	CHECK( array2.get_descriptor() == descriptor );
	CHECK( array1.get_storage() == storage.get() );
	CHECK( array2.get_storage() == storage.get() );
}

TEST_CASE("Calling empty with an output array with a different descriptor should alias storage", "[array_factory]")
{
	const auto context = make_test_device_context();

	const std::array<std::size_t, 3> extents1 = {80, 16, 256};
	const array_descriptor descriptor1(
		strided_layout::make_contiguous_layout(make_span(extents1)),
		numerical_type::float32
	);
	const std::array<std::size_t, 3> extents2 = {40, 32, 256};
	const array_descriptor descriptor2(
		strided_layout::make_contiguous_layout(make_span(extents2)),
		numerical_type::float32
	);

	const auto alloc_bytes = compute_storage_requirement(descriptor1);
	const auto storage = 
		std::make_shared<hardware::host_buffer>(alloc_bytes, 256);

	const auto affinity = GENERATE(
		hardware::memory_resource_affinity::device,
		hardware::memory_resource_affinity::host
	);
	auto &allocator = dynamic_cast<hardware::mock_memory_allocator&>(
		context.get_memory_allocator(affinity)
	);
	REQUIRE_CALL(allocator, get_memory_resource())
		.LR_RETURN(hardware::get_host_memory_resource());

	array array1(storage, descriptor1);
	const auto array2 = empty(descriptor2, affinity, context, &array1);

	CHECK( array1.get_descriptor() == descriptor1 );
	CHECK( array2.get_descriptor() == descriptor2 );
	CHECK( array1.get_storage() == storage.get() );
	CHECK( array2.get_storage() == storage.get() );
}

TEST_CASE("Calling empty with an output array with no storage should allocate it", "[array_factory]")
{
	const auto context = make_test_device_context();

	const std::array<std::size_t, 3> extents1 = {80, 16, 256};
	const array_descriptor descriptor1(
		strided_layout::make_contiguous_layout(make_span(extents1)),
		numerical_type::float32
	);
	const std::array<std::size_t, 3> extents2 = {40, 32, 256};
	const array_descriptor descriptor2(
		strided_layout::make_contiguous_layout(make_span(extents2)),
		numerical_type::float32
	);

	const auto alloc_bytes = compute_storage_requirement(descriptor2);
	const auto buffer = 
		std::make_shared<hardware::host_buffer>(alloc_bytes, 256);

	const auto affinity = GENERATE(
		hardware::memory_resource_affinity::device,
		hardware::memory_resource_affinity::host
	);

	auto &allocator = dynamic_cast<hardware::mock_memory_allocator&>(
		context.get_memory_allocator(affinity)
	);

	REQUIRE_CALL(allocator, get_max_alignment())
		.RETURN(256);
	REQUIRE_CALL(allocator, allocate(alloc_bytes, 256, nullptr))
		.RETURN(buffer);

	array array1(nullptr, descriptor1);
	const auto array2 = empty(descriptor2, affinity, context, &array1);

	CHECK( array1.get_descriptor() == descriptor1 );
	CHECK( array2.get_descriptor() == descriptor2 );
	CHECK( array1.get_storage() == nullptr );
	CHECK( array2.get_storage() == buffer.get() );
}

TEST_CASE("Calling empty with a output array with insufficient storage should allocate it", "[array_factory]")
{
	const auto context = make_test_device_context();

	const std::array<std::size_t, 3> extents1 = {8, 16, 256};
	const array_descriptor descriptor1(
		strided_layout::make_contiguous_layout(make_span(extents1)),
		numerical_type::float32
	);
	const std::array<std::size_t, 3> extents2 = {40, 32, 256};
	const array_descriptor descriptor2(
		strided_layout::make_contiguous_layout(make_span(extents2)),
		numerical_type::float32
	);

	const auto alloc_bytes = compute_storage_requirement(descriptor2);
	const auto buffer1 = 
		std::make_shared<hardware::host_buffer>(alloc_bytes-1, 256);
	const auto buffer2 = 
		std::make_shared<hardware::host_buffer>(alloc_bytes, 256);

	const auto affinity = GENERATE(
		hardware::memory_resource_affinity::device,
		hardware::memory_resource_affinity::host
	);

	auto &allocator = dynamic_cast<hardware::mock_memory_allocator&>(
		context.get_memory_allocator(affinity)
	);

	REQUIRE_CALL(allocator, get_max_alignment())
		.RETURN(256);
	REQUIRE_CALL(allocator, allocate(alloc_bytes, 256, nullptr))
		.RETURN(buffer2);

	array array1(buffer1, descriptor1);
	const auto array2 = empty(descriptor2, affinity, context, &array1);

	CHECK( array1.get_descriptor() == descriptor1 );
	CHECK( array2.get_descriptor() == descriptor2 );
	CHECK( array1.get_storage() == buffer1.get() );
	CHECK( array2.get_storage() == buffer2.get() );
}

TEST_CASE("Calling empty with an output array with a storage in a different memory resource should allocate", "[array_factory]")
{
	const auto context = make_test_device_context();

	const std::array<std::size_t, 3> extents1 = {80, 16, 256};
	const array_descriptor descriptor1(
		strided_layout::make_contiguous_layout(make_span(extents1)),
		numerical_type::float32
	);
	const std::array<std::size_t, 3> extents2 = {40, 32, 256};
	const array_descriptor descriptor2(
		strided_layout::make_contiguous_layout(make_span(extents2)),
		numerical_type::float32
	);

	const auto alloc_bytes = compute_storage_requirement(descriptor1);

	hardware::mock_memory_resource resource1;
	hardware::mock_memory_resource resource2;
	const auto buffer1 = std::make_shared<hardware::buffer>(
		nullptr, 
		alloc_bytes, 
		resource1, 
		nullptr
	);
	const auto buffer2 = 
		std::make_shared<hardware::host_buffer>(alloc_bytes, 256);

	const auto affinity = GENERATE(
		hardware::memory_resource_affinity::device,
		hardware::memory_resource_affinity::host
	);

	auto &allocator = dynamic_cast<hardware::mock_memory_allocator&>(
		context.get_memory_allocator(affinity)
	);

	REQUIRE_CALL(allocator, get_memory_resource())
		.LR_RETURN(resource2);
	REQUIRE_CALL(allocator, get_max_alignment())
		.RETURN(256);
	REQUIRE_CALL(allocator, allocate(alloc_bytes, 256, nullptr))
		.RETURN(buffer2);

	array array1(buffer1, descriptor1);
	const auto array2 = empty(descriptor2, affinity, context, &array1);

	CHECK( array1.get_descriptor() == descriptor1 );
	CHECK( array2.get_descriptor() == descriptor2 );
	CHECK( array1.get_storage() == buffer1.get() );
	CHECK( array2.get_storage() == buffer2.get() );
}
