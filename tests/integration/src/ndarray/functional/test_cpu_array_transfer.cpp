// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/ndarray/functional/array_transfer.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/ndarray/functional/array_creation.hpp>
#include <xmipp4/core/dispatch/execution_context.hpp>
#include <xmipp4/core/dispatch/program_manager.hpp>
#include <xmipp4/core/dispatch/dispatcher.hpp>
#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/array_view.hpp>
#include <xmipp4/core/ndarray/array_descriptor.hpp>
#include <xmipp4/core/layout/strided_layout.hpp>
#include <xmipp4/core/hardware/device_manager.hpp>
#include <xmipp4/core/hardware/device_index.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/scalar_value.hpp>
#include <xmipp4/core/span.hpp>

#include <cstddef>
#include <vector>

using namespace xmipp4;

namespace
{

class cpu_execution_context_fixture
{
public:
	cpu_execution_context_fixture()
	{
		const auto device_manager =
			catalog.get_service_manager<xmipp4::device_manager>();
		const auto instance = device_manager->create_device_instance(
			device_index("cpu", 0)
		);
		const auto program_manager =
			catalog.get_service_manager<xmipp4::program_manager>();
		context = execution_context(
			device_context(instance),
			make_eager_dispatcher(program_manager)
		);
	}

protected:
	array_descriptor make_descriptor(
		std::vector<std::size_t> extents = { 2, 3 },
		numerical_type data_type = numerical_type::float32
	) const
	{
		return array_descriptor(
			strided_layout::make_contiguous_layout(make_span(extents)),
			data_type
		);
	}

	// Reads back the first count elements of a host-accessible array as type T.
	template <typename T>
	std::vector<T> read_host(const array &arr, std::size_t count) const
	{
		const auto *storage = arr.get_storage();
		REQUIRE( storage != nullptr );

		const auto *data = static_cast<const T*>(storage->get_host_ptr());
		REQUIRE( data != nullptr );

		return std::vector<T>(data, data + count);
	}

	service_catalog catalog;
	execution_context context;
};

} // namespace


TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"transfer to device aliases the input on CPU because host and device "
	"resources coincide",
	"[array_transfer][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto source = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(2.0f),
		context
	);

	const auto result = transfer(
		source,
		memory_resource_affinity::device,
		context
	);

	CHECK( result.get_descriptor() == descriptor );

	// On CPU the device resource aliases the host resource, so no copy is made:
	// the result shares the source storage.
	CHECK( result.get_storage() == source.get_storage() );

	const auto values = read_host<float>(result, 6);
	for (const auto value : values)
	{
		CHECK( value == 2.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"transfer to host aliases the input on CPU",
	"[array_transfer][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto source = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(5.0f),
		context
	);

	const auto result = transfer(
		source,
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_descriptor() == descriptor );

	// The source already lives on host-accessible memory, so it is aliased.
	CHECK( result.get_storage() == source.get_storage() );

	const auto values = read_host<float>(result, 6);
	for (const auto value : values)
	{
		CHECK( value == 5.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"transfer_copy duplicates the source into independent storage on CPU",
	"[array_transfer][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto source = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(3.0f),
		context
	);

	const auto result = transfer_copy(
		source,
		memory_resource_affinity::device,
		context
	);

	CHECK( result.get_descriptor() == descriptor );

	// A forced copy always owns a distinct buffer from the source, even though
	// the CPU host and device resources coincide.
	CHECK( result.get_storage() != source.get_storage() );

	const auto values = read_host<float>(result, 6);
	for (const auto value : values)
	{
		CHECK( value == 3.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"transfer_copy reuses the storage of the provided output array on CPU",
	"[array_transfer][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto source = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(6.0f),
		context
	);

	// Pre-allocate a compatible output; its storage should be reused.
	auto out = zeros(
		descriptor,
		memory_resource_affinity::host,
		context
	);
	const auto *out_storage = out.get_storage();

	const auto result = transfer_copy(
		source,
		memory_resource_affinity::host,
		context,
		&out
	);

	CHECK( result.get_storage() == out_storage );
	CHECK( result.get_storage() != source.get_storage() );

	const auto values = read_host<float>(result, 6);
	for (const auto value : values)
	{
		CHECK( value == 6.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"to_device and to_host alias the input on CPU",
	"[array_transfer][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto source = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(1.5f),
		context
	);

	const auto on_device = to_device(source, context);
	CHECK( on_device.get_storage() == source.get_storage() );

	const auto on_host = to_host(source, context);
	CHECK( on_host.get_storage() == source.get_storage() );

	const auto values = read_host<float>(on_device, 6);
	for (const auto value : values)
	{
		CHECK( value == 1.5f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"to_device_copy and to_host_copy duplicate into independent storage on CPU",
	"[array_transfer][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto source = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(4.0f),
		context
	);

	const auto on_device = to_device_copy(source, context);
	CHECK( on_device.get_storage() != source.get_storage() );

	const auto on_host = to_host_copy(source, context);
	CHECK( on_host.get_storage() != source.get_storage() );
	CHECK( on_host.get_storage() != on_device.get_storage() );

	for (const auto value : read_host<float>(on_device, 6))
	{
		CHECK( value == 4.0f );
	}
	for (const auto value : read_host<float>(on_host, 6))
	{
		CHECK( value == 4.0f );
	}
}
