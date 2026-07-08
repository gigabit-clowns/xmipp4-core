// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/array_cast.hpp>

#include <xmipp4/core/service_catalog.hpp>
#include <xmipp4/core/multidimensional/array_creation.hpp>
#include <xmipp4/core/execution/context.hpp>
#include <xmipp4/core/execution/program_manager.hpp>
#include <xmipp4/core/execution/dispatcher.hpp>
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
#include <cstdint>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;
using namespace xmipp4::execution;
using namespace xmipp4::ndarray;
using namespace xmipp4::layout;

namespace
{

class cpu_context_fixture
{
public:
	cpu_context_fixture()
	{
		const auto device_manager =
			catalog.get_service_manager<hardware::device_manager>();
		const auto instance = device_manager->create_device_instance(
			hardware::device_index("cpu", 0)
		);
		const auto program_manager =
			catalog.get_service_manager<execution::program_manager>();
		context = execution::context(
			hardware::device_context(instance),
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
	execution::context context;
};

} // namespace



TEST_CASE_METHOD(
	cpu_context_fixture,
	"cast aliases the input when it already has the requested type",
	"[array_cast][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::float32);

	auto source = full(
		descriptor,
		hardware::memory_resource_affinity::device,
		scalar_value(2.0f),
		context
	);

	const auto result = cast(source, numerical_type::float32, context);

	// The type already matches, so the result shares the source storage.
	CHECK( result.get_storage() == source.get_storage() );
	CHECK( result.get_descriptor() == descriptor );

	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 2.0f );
	}
}

TEST_CASE_METHOD(
	cpu_context_fixture,
	"cast converts the elements into an independent array when the type differs",
	"[array_cast][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::float32);

	auto source = full(
		descriptor,
		hardware::memory_resource_affinity::device,
		scalar_value(2.5f),
		context
	);

	const auto result = cast(source, numerical_type::float64, context);

	// A conversion is required, so a distinct buffer holding the target type is
	// produced.
	CHECK( result.get_storage() != source.get_storage() );
	CHECK( result.get_descriptor().get_data_type() == numerical_type::float64 );

	for (const auto value : read_host<double>(result, 6))
	{
		CHECK( value == 2.5 );
	}
}

TEST_CASE_METHOD(
	cpu_context_fixture,
	"cast truncates towards zero when converting to an integral type",
	"[array_cast][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::float32);

	auto source = full(
		descriptor,
		hardware::memory_resource_affinity::device,
		scalar_value(3.75f),
		context
	);

	const auto result = cast(source, numerical_type::int32, context);

	CHECK( result.get_descriptor().get_data_type() == numerical_type::int32 );

	for (const auto value : read_host<std::int32_t>(result, 6))
	{
		CHECK( value == 3 );
	}
}

TEST_CASE_METHOD(
	cpu_context_fixture,
	"cast_copy duplicates into independent storage even when the type matches",
	"[array_cast][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::float32);

	const auto source = full(
		descriptor,
		hardware::memory_resource_affinity::device,
		scalar_value(4.0f),
		context
	);

	const auto result = cast_copy(source, numerical_type::float32, context);

	// A forced copy always owns a distinct buffer from the source.
	CHECK( result.get_storage() != source.get_storage() );
	CHECK( result.get_descriptor().get_data_type() == numerical_type::float32 );

	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 4.0f );
	}
}

TEST_CASE_METHOD(
	cpu_context_fixture,
	"cast_copy converts the elements to the requested type",
	"[array_cast][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::float32);

	const auto source = full(
		descriptor,
		hardware::memory_resource_affinity::device,
		scalar_value(7.0f),
		context
	);

	const auto result = cast_copy(source, numerical_type::float64, context);

	CHECK( result.get_storage() != source.get_storage() );
	CHECK( result.get_descriptor().get_data_type() == numerical_type::float64 );

	for (const auto value : read_host<double>(result, 6))
	{
		CHECK( value == 7.0 );
	}
}

TEST_CASE_METHOD(
	cpu_context_fixture,
	"cast_copy reuses the storage of the provided output array",
	"[array_cast][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::float32);
	const auto target_descriptor =
		make_descriptor({ 2, 3 }, numerical_type::float64);

	const auto source = full(
		descriptor,
		hardware::memory_resource_affinity::device,
		scalar_value(5.0f),
		context
	);

	// Pre-allocate a compatible output; its storage should be reused.
	auto out = zeros(
		target_descriptor,
		hardware::memory_resource_affinity::device,
		context
	);
	const auto *out_storage = out.get_storage();

	const auto result = cast_copy(
		source,
		numerical_type::float64,
		context,
		&out
	);

	CHECK( result.get_storage() == out_storage );
	CHECK( result.get_storage() != source.get_storage() );
	CHECK( result.get_descriptor().get_data_type() == numerical_type::float64 );

	for (const auto value : read_host<double>(result, 6))
	{
		CHECK( value == 5.0 );
	}
}
