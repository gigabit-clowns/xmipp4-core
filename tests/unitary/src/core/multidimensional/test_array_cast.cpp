// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/array_cast.hpp>

#include <xmipp4/core/multidimensional/execution_context.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operations/assignment/copy_operation.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/device_instance.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>

#include "mock/mock_operation_dispatcher.hpp"
#include "../hardware/mock/mock_device.hpp"
#include "../hardware/mock/mock_memory_resource.hpp"
#include "../hardware/mock/mock_memory_allocator.hpp"
#include "../hardware/mock/mock_command_queue.hpp"
#include "../hardware/mock/mock_buffer.hpp"

#include <cstddef>
#include <memory>
#include <typeinfo>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;
using trompeloeil::_;

namespace
{

// Records the arguments seen by a mocked dispatch() call so that they can be
// inspected after the function under test has returned.
struct dispatch_record
{
	bool called = false;
	const std::type_info *operation_type = nullptr;
	std::size_t num_outputs = 0;
	std::size_t num_inputs = 0;
	const hardware::buffer *first_output_storage = nullptr;
	const hardware::buffer *first_input_storage = nullptr;
	const hardware::device_instance *device_instance = nullptr;

	void operator()(
		const operation &op,
		span<array> outputs,
		span<const array_view> inputs,
		const hardware::device_context &device_context
	)
	{
		called = true;
		operation_type = &typeid(op);
		num_outputs = outputs.size();
		num_inputs = inputs.size();
		if (!outputs.empty())
		{
			first_output_storage = outputs[0].get_storage();
		}
		if (!inputs.empty())
		{
			first_input_storage = inputs[0].get_storage();
		}
		device_instance = device_context.get_device_instance().get();
	}
};

class array_cast_fixture
{
public:
	array_cast_fixture()
		: device(std::make_shared<hardware::mock_device>())
		, host_allocator(std::make_shared<hardware::mock_memory_allocator>())
		, device_allocator(std::make_shared<hardware::mock_memory_allocator>())
		, default_queue(std::make_shared<hardware::mock_command_queue>())
		, dispatcher(std::make_shared<mock_operation_dispatcher>())
	{
		using hardware::memory_resource_affinity;

		hardware::device_properties properties;
		properties.set_optimal_data_alignment(128);

		REQUIRE_CALL(
			*device,
			get_memory_resource(memory_resource_affinity::host)
		)
			.LR_RETURN(host_resource);
		REQUIRE_CALL(
			*device,
			get_memory_resource(memory_resource_affinity::device)
		)
			.LR_RETURN(device_resource);
		REQUIRE_CALL(host_resource, create_allocator())
			.RETURN(host_allocator);
		REQUIRE_CALL(device_resource, create_allocator())
			.RETURN(device_allocator);
		REQUIRE_CALL(*device, create_command_queue())
			.RETURN(default_queue);

		instance = std::make_shared<hardware::device_instance>(
			device,
			std::move(properties)
		);

		context = execution_context(
			hardware::device_context(instance),
			dispatcher
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

	std::shared_ptr<hardware::mock_device> device;
	std::shared_ptr<hardware::mock_memory_allocator> host_allocator;
	std::shared_ptr<hardware::mock_memory_allocator> device_allocator;
	hardware::mock_memory_resource host_resource;
	hardware::mock_memory_resource device_resource;
	std::shared_ptr<hardware::command_queue> default_queue;
	std::shared_ptr<const hardware::device_instance> instance;
	std::shared_ptr<mock_operation_dispatcher> dispatcher;
	execution_context context;
};

} // namespace



TEST_CASE_METHOD(
	array_cast_fixture,
	"cast returns an alias of the input without allocating or dispatching when "
	"the array already has the requested type",
	"[array_cast]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::float32);
	const auto buffer = std::make_shared<hardware::mock_buffer>();
	array input(buffer, descriptor);

	// Neither the allocators nor the dispatcher are expected to be used; an
	// unexpected call on any of them would fail the test.
	const auto result = cast(input, numerical_type::float32, context);

	CHECK( result.get_storage() == buffer.get() );
	CHECK( result.get_descriptor() == descriptor );
}

TEST_CASE_METHOD(
	array_cast_fixture,
	"cast allocates a new device array and dispatches a copy_operation when the "
	"requested type differs",
	"[array_cast]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::float32);
	const auto source_buffer = std::make_shared<hardware::mock_buffer>();
	const auto buffer = std::make_shared<hardware::mock_buffer>();
	array input(source_buffer, descriptor);

	ALLOW_CALL(*device_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*device_allocator,
		allocate(_, _, default_queue.get())
	)
		.RETURN(buffer);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = cast(input, numerical_type::float64, context);

	CHECK( result.get_storage() == buffer.get() );
	CHECK( result.get_descriptor().get_data_type() == numerical_type::float64 );

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(copy_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 1 );
	CHECK( record.first_output_storage == buffer.get() );
	CHECK( record.first_input_storage == source_buffer.get() );
	CHECK( record.device_instance == instance.get() );
}

TEST_CASE_METHOD(
	array_cast_fixture,
	"cast_copy allocates a new device array and dispatches a copy_operation "
	"even when the input already has the requested type",
	"[array_cast]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::float32);
	const auto source_buffer = std::make_shared<hardware::mock_buffer>();
	const auto buffer = std::make_shared<hardware::mock_buffer>();
	array input(source_buffer, descriptor);

	ALLOW_CALL(*device_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*device_allocator,
		allocate(_, _, default_queue.get())
	)
		.RETURN(buffer);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = cast_copy(input.share(), numerical_type::float32, context);

	CHECK( result.get_storage() == buffer.get() );
	CHECK( result.get_descriptor().get_data_type() == numerical_type::float32 );

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(copy_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 1 );
	CHECK( record.first_output_storage == buffer.get() );
	CHECK( record.first_input_storage == source_buffer.get() );
	CHECK( record.device_instance == instance.get() );
}

TEST_CASE_METHOD(
	array_cast_fixture,
	"cast_copy casts to the requested type and reuses a compatible output array",
	"[array_cast]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::float32);
	const auto target_descriptor =
		make_descriptor({ 2, 3 }, numerical_type::float64);
	const auto size = compute_storage_requirement(target_descriptor);
	const auto source_buffer = std::make_shared<hardware::mock_buffer>();
	const auto out_buffer = std::make_shared<hardware::mock_buffer>();
	array input(source_buffer, descriptor);

	// The output buffer lives on the device resource and is large enough, so it
	// is reused verbatim (no allocate() call).
	ALLOW_CALL(*out_buffer, get_memory_resource())
		.LR_RETURN(device_resource);
	ALLOW_CALL(*out_buffer, get_size())
		.RETURN(size);
	ALLOW_CALL(*device_allocator, get_memory_resource())
		.LR_RETURN(device_resource);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	array out(out_buffer, target_descriptor);

	const auto result = cast_copy(
		input.share(),
		numerical_type::float64,
		context,
		&out
	);

	CHECK( result.get_storage() == out_buffer.get() );
	CHECK( out.get_storage() == out_buffer.get() );
	CHECK( result.get_descriptor().get_data_type() == numerical_type::float64 );

	CHECK( record.called );
	CHECK( *record.operation_type == typeid(copy_operation) );
	CHECK( record.first_output_storage == out_buffer.get() );
	CHECK( record.first_input_storage == source_buffer.get() );
}
