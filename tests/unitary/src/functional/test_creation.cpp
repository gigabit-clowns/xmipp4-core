// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/functional/creation.hpp>

#include <xmipp4/core/dispatch/execution_context.hpp>
#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/const_array.hpp>
#include <xmipp4/core/ndarray/array_descriptor.hpp>
#include <xmipp4/core/layout/strided_layout.hpp>
#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/ops/assignment/fill_operation.hpp>
#include <xmipp4/ops/assignment/copy_operation.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/device_session.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>

#include "../core/dispatch/mock/mock_dispatcher.hpp"
#include "../core/hardware/mock/mock_device.hpp"
#include "../core/hardware/mock/mock_memory_resource.hpp"
#include "../core/hardware/mock/mock_memory_allocator.hpp"
#include "../core/hardware/mock/mock_command_queue.hpp"
#include "../core/hardware/mock/mock_buffer.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::ops;
using trompeloeil::_;

namespace
{

// Records the arguments seen by a mocked dispatch() call so that they can be
// inspected after the function under test has returned. The fill value and
// operation type are captured by value to stay valid past the operation's
// (temporary) lifetime.
struct dispatch_record
{
	bool called = false;
	const std::type_info *operation_type = nullptr;
	std::unique_ptr<scalar_value> fill_value;
	std::size_t num_outputs = 0;
	std::size_t num_inputs = 0;
	const buffer *first_output_storage = nullptr;
	const buffer *first_input_storage = nullptr;
	const xmipp4::device_session *device_session = nullptr;

	void operator()(
		const operation &op,
		span<array> outputs,
		span<const const_array> inputs,
		const device_context &device_context
	)
	{
		called = true;
		operation_type = &typeid(op);
		num_outputs = outputs.size();
		num_inputs = inputs.size();
		if (const auto *fill = dynamic_cast<const fill_operation*>(&op))
		{
			fill_value =
				std::make_unique<scalar_value>(fill->get_fill_value());
		}
		if (!outputs.empty())
		{
			first_output_storage = outputs[0].get_storage();
		}
		if (!inputs.empty())
		{
			first_input_storage = inputs[0].get_storage();
		}
		device_session = device_context.get_device_session().get();
	}
};

class array_creation_fixture
{
public:
	array_creation_fixture()
		: device(std::make_shared<mock_device>())
		, host_allocator(std::make_shared<mock_memory_allocator>())
		, device_allocator(std::make_shared<mock_memory_allocator>())
		, default_queue(std::make_shared<mock_command_queue>())
		, dispatcher(std::make_shared<mock_dispatcher>())
	{

		device_properties properties;
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

		session = std::make_shared<device_session>(
			device,
			std::move(properties)
		);

		context = execution_context(
			device_context(session),
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

	std::shared_ptr<mock_device> device;
	std::shared_ptr<mock_memory_allocator> host_allocator;
	std::shared_ptr<mock_memory_allocator> device_allocator;
	mock_memory_resource host_resource;
	mock_memory_resource device_resource;
	std::shared_ptr<command_queue> default_queue;
	std::shared_ptr<const device_session> session;
	std::shared_ptr<mock_dispatcher> dispatcher;
	execution_context context;
};

} // namespace


TEST_CASE(
	"empty throws when the execution context has no allocator for the "
	"requested affinity",
	"[array_creation]"
)
{
	const std::vector<std::size_t> extents = { 2, 3 };
	const array_descriptor descriptor(
		strided_layout::make_contiguous_layout(make_span(extents)),
		numerical_type::float32
	);

	// A default-constructed context is empty: it has no allocators.
	const execution_context context;

	CHECK_THROWS_AS(
		empty(
			descriptor,
			memory_resource_affinity::host,
			context
		),
		std::invalid_argument
	);
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"empty allocates storage from the affinity allocator and wraps it in an "
	"array carrying the requested descriptor",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto buffer = std::make_shared<mock_buffer>();

	ALLOW_CALL(*host_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*host_allocator,
		allocate(size, _, default_queue.get())
	)
		.RETURN(buffer);

	const auto result = empty(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_storage() == buffer.get() );
	CHECK( result.get_descriptor() == descriptor );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"empty routes the allocation to the device allocator for device affinity",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto buffer = std::make_shared<mock_buffer>();

	// Only the device allocator is expected to be used; an unexpected call on
	// the host allocator would fail the test.
	ALLOW_CALL(*device_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*device_allocator,
		allocate(size, _, default_queue.get())
	)
		.RETURN(buffer);

	const auto result = empty(
		descriptor,
		memory_resource_affinity::device,
		context
	);

	CHECK( result.get_storage() == buffer.get() );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"empty reuses the storage of a compatible output array instead of "
	"allocating a new buffer",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto buffer = std::make_shared<mock_buffer>();

	// The output buffer lives on the same resource as the allocator and is
	// large enough, so it should be reused verbatim (no allocate() call).
	ALLOW_CALL(*buffer, get_memory_resource())
		.LR_RETURN(host_resource);
	ALLOW_CALL(*buffer, get_size())
		.RETURN(size);
	ALLOW_CALL(*host_allocator, get_memory_resource())
		.LR_RETURN(host_resource);

	array out(buffer, descriptor);

	const auto result = empty(
		descriptor,
		memory_resource_affinity::host,
		context,
		&out
	);

	CHECK( result.get_storage() == buffer.get() );
	CHECK( out.get_storage() == buffer.get() );

	// The descriptor already matched, so the output array is left untouched.
	CHECK( out.get_descriptor() == descriptor );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"empty reuses compatible storage but overrides the output array when its "
	"descriptor differs",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto stale_descriptor = make_descriptor({ 1 });
	const auto buffer = std::make_shared<mock_buffer>();

	// Same resource and large enough, so the buffer is reused, but the stale
	// descriptor must be replaced by the requested one.
	ALLOW_CALL(*buffer, get_memory_resource())
		.LR_RETURN(host_resource);
	ALLOW_CALL(*buffer, get_size())
		.RETURN(size);
	ALLOW_CALL(*host_allocator, get_memory_resource())
		.LR_RETURN(host_resource);

	array out(buffer, stale_descriptor);

	const auto result = empty(
		descriptor,
		memory_resource_affinity::host,
		context,
		&out
	);

	// The storage is reused (no allocation), but the descriptor is overridden.
	CHECK( result.get_storage() == buffer.get() );
	CHECK( out.get_storage() == buffer.get() );
	CHECK( out.get_descriptor() == descriptor );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"empty allocates and overrides the output array when it has no storage",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto buffer = std::make_shared<mock_buffer>();

	// A default-constructed output has no storage to reuse.
	ALLOW_CALL(*host_allocator, get_memory_resource())
		.LR_RETURN(host_resource);
	ALLOW_CALL(*host_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*host_allocator,
		allocate(size, _, default_queue.get())
	)
		.RETURN(buffer);

	array out;

	const auto result = empty(
		descriptor,
		memory_resource_affinity::host,
		context,
		&out
	);

	CHECK( result.get_storage() == buffer.get() );
	CHECK( out.get_storage() == buffer.get() );
	CHECK( out.get_descriptor() == descriptor );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"empty reallocates and overrides the output array when its storage lives "
	"on a different memory resource",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto stale_buffer = std::make_shared<mock_buffer>();
	const auto buffer = std::make_shared<mock_buffer>();

	// The stale buffer lives on the device resource, while the host allocator
	// hands out host-resource storage; reuse must be rejected.
	ALLOW_CALL(*stale_buffer, get_memory_resource())
		.LR_RETURN(device_resource);
	ALLOW_CALL(*host_allocator, get_memory_resource())
		.LR_RETURN(host_resource);
	ALLOW_CALL(*host_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*host_allocator,
		allocate(size, _, default_queue.get())
	)
		.RETURN(buffer);

	array out(stale_buffer, descriptor);

	const auto result = empty(
		descriptor,
		memory_resource_affinity::host,
		context,
		&out
	);

	// A fresh buffer is allocated and the output array is overridden with it.
	CHECK( result.get_storage() == buffer.get() );
	CHECK( out.get_storage() == buffer.get() );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"empty reallocates and overrides the output array when its storage is too "
	"small",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto stale_buffer = std::make_shared<mock_buffer>();
	const auto buffer = std::make_shared<mock_buffer>();

	// The stale buffer is on the right resource but undersized; reuse must be
	// rejected in favor of a fresh allocation.
	ALLOW_CALL(*stale_buffer, get_memory_resource())
		.LR_RETURN(host_resource);
	ALLOW_CALL(*stale_buffer, get_size())
		.RETURN(size - 1);
	ALLOW_CALL(*host_allocator, get_memory_resource())
		.LR_RETURN(host_resource);
	ALLOW_CALL(*host_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*host_allocator,
		allocate(size, _, default_queue.get())
	)
		.RETURN(buffer);

	array out(stale_buffer, descriptor);

	const auto result = empty(
		descriptor,
		memory_resource_affinity::host,
		context,
		&out
	);

	CHECK( result.get_storage() == buffer.get() );
	CHECK( out.get_storage() == buffer.get() );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"full allocates an output array and dispatches a fill_operation carrying "
	"the requested value",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto buffer = std::make_shared<mock_buffer>();

	ALLOW_CALL(*host_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*host_allocator,
		allocate(size, _, default_queue.get())
	)
		.RETURN(buffer);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(2.5f),
		context
	);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(fill_operation) );
	REQUIRE( record.fill_value != nullptr );
	CHECK( record.fill_value->get<float>() == 2.5f );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 0 );
	CHECK( record.first_output_storage == buffer.get() );
	CHECK( record.device_session == session.get() );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"zeros dispatches a fill_operation with a zero fill value",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto buffer = std::make_shared<mock_buffer>();

	ALLOW_CALL(*host_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*host_allocator,
		allocate(_, _, default_queue.get())
	)
		.RETURN(buffer);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = zeros(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(fill_operation) );
	REQUIRE( record.fill_value != nullptr );
	CHECK( record.fill_value->get<int>() == 0 );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 0 );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"ones dispatches a fill_operation with a one fill value",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto buffer = std::make_shared<mock_buffer>();

	ALLOW_CALL(*host_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*host_allocator,
		allocate(_, _, default_queue.get())
	)
		.RETURN(buffer);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = ones(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(fill_operation) );
	REQUIRE( record.fill_value != nullptr );
	CHECK( record.fill_value->get<int>() == 1 );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 0 );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"copy dispatches a copy_operation with the source as the single input",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto source_buffer = std::make_shared<mock_buffer>();
	array source(source_buffer, descriptor);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = copy(source.share(), context);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(copy_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 1 );
	CHECK( record.first_input_storage == source_buffer.get() );
	CHECK( record.device_session == session.get() );
}

TEST_CASE_METHOD(
	array_creation_fixture,
	"fill dispatches a fill_operation on the provided array with no inputs",
	"[array_creation]"
)
{
	const auto descriptor = make_descriptor();
	const auto buffer = std::make_shared<mock_buffer>();
	array target(buffer, descriptor);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	fill(target, scalar_value(7), context);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(fill_operation) );
	REQUIRE( record.fill_value != nullptr );
	CHECK( record.fill_value->get<int>() == 7 );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 0 );
	CHECK( record.first_output_storage == buffer.get() );
}
