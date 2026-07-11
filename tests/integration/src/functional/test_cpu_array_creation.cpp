// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/functional/array_creation.hpp>

#include <xmipp4/core/service_catalog.hpp>
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
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>
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
	"empty produces a host-accessible array carrying the requested descriptor",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto result = empty(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_descriptor() == descriptor );

	// The storage must exist and be reachable from the host, even though its
	// contents are left uninitialized.
	const auto *storage = result.get_storage();
	REQUIRE( storage != nullptr );
	CHECK( storage->get_host_ptr() != nullptr );
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"zeros sets every element of a host array to zero",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto result = zeros(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_descriptor() == descriptor );

	const auto values = read_host<float>(result, 6);
	for (const auto value : values)
	{
		CHECK( value == 0.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"ones sets every element of a host array to one",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto result = ones(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_descriptor() == descriptor );

	const auto values = read_host<float>(result, 6);
	for (const auto value : values)
	{
		CHECK( value == 1.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"full sets every element of a host array to the requested value",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 4 });

	const auto result = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(3.5f),
		context
	);

	const auto values = read_host<float>(result, 4);
	for (const auto value : values)
	{
		CHECK( value == 3.5f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"fill overwrites the contents of an existing host array",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 5 });

	// Start from zeros so a successful fill is unambiguous.
	auto result = zeros(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	fill(result, scalar_value(7.0f), context);

	const auto values = read_host<float>(result, 5);
	for (const auto value : values)
	{
		CHECK( value == 7.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"copy duplicates the source contents into independent storage",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto source = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(2.0f),
		context
	);

	const auto result = copy(source, context);

	CHECK( result.get_descriptor() == descriptor );

	// The copy owns a distinct buffer from the source.
	CHECK( result.get_storage() != source.get_storage() );

	const auto values = read_host<float>(result, 6);
	for (const auto value : values)
	{
		CHECK( value == 2.0f );
	}
}
