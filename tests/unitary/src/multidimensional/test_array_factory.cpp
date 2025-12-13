// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/array_factory.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/device_index.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/memory_allocator_manager.hpp>
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


hardware::device_context make_test_device_context(
	std::shared_ptr<hardware::memory_allocator> device_optimal_allocator,
	std::shared_ptr<hardware::memory_allocator> host_accessible_allocator
)
{
	const hardware::device_index index("mock", 1234);

	auto device_backend = std::make_unique<hardware::mock_device_backend>();
	auto device = std::make_shared<hardware::mock_device>();
	hardware::mock_memory_resource device_local_memory_resource;
	hardware::mock_memory_resource host_accessible_memory_resource;
	auto allocator_backend = 
		std::make_unique<hardware::mock_memory_allocator_backend>();

	REQUIRE_CALL(*device_backend, get_name())
		.RETURN("mock");
	REQUIRE_CALL(*device_backend, create_device(index.get_device_id()))
		.RETURN(device);
	
	REQUIRE_CALL(*device, get_device_local_memory_resource())
		.LR_RETURN(device_local_memory_resource);
	REQUIRE_CALL(*device, get_host_accessible_memory_resource())
		.LR_RETURN(host_accessible_memory_resource);

	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const hardware::memory_resource&)))
		.LR_WITH(&_1 == &device_local_memory_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, get_suitability(ANY(const hardware::memory_resource&)))
		.LR_WITH(&_1 == &host_accessible_memory_resource)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(hardware::memory_resource&)))
		.LR_WITH(&_1 == &device_local_memory_resource)
		.RETURN(device_optimal_allocator);
	REQUIRE_CALL(*allocator_backend, create_memory_allocator(ANY(hardware::memory_resource&)))
		.LR_WITH(&_1 == &host_accessible_memory_resource)
		.RETURN(host_accessible_allocator);

	service_catalog catalog(false);
	catalog.get_service_manager<hardware::device_manager>()
		.register_backend(std::move(device_backend));
	catalog.get_service_manager<hardware::memory_allocator_manager>()
		.register_backend(std::move(allocator_backend));

	return hardware::device_context(catalog, index);
}

TEST_CASE("Calling empty without an output array should allocate with the appropiate allocator.", "[array_factory]")
{
	const auto device_optimal_allocator = 
		std::make_shared<hardware::mock_memory_allocator>();
	const auto host_accessible_allocator = 
		std::make_shared<hardware::mock_memory_allocator>();
	const auto context = make_test_device_context(
		device_optimal_allocator, 
		host_accessible_allocator
	);

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
	const auto buffer = std::make_shared<hardware::host_buffer>(alloc_bytes, 256);

	array result;
	SECTION ("device optimal placement")
	{
		REQUIRE_CALL(*device_optimal_allocator, get_max_alignment())
			.RETURN(256);
		REQUIRE_CALL(*device_optimal_allocator, allocate(alloc_bytes, 256, nullptr))
			.RETURN(buffer);

		result = empty(
			descriptor,
			hardware::target_placement::device_optimal,
			context,
			nullptr
		);
	}

	SECTION ("host accessible placement")
	{
		REQUIRE_CALL(*host_accessible_allocator, get_max_alignment())
			.RETURN(256);
		REQUIRE_CALL(*host_accessible_allocator, allocate(alloc_bytes, 256, nullptr))
			.RETURN(buffer);

		result = empty(
			descriptor,
			hardware::target_placement::host_accessible,
			context,
			nullptr
		);
	}
	
	CHECK( result.get_descriptor() == descriptor );
	CHECK( result.get_storage() == buffer.get() );
}
