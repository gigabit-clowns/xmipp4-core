// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/ndarray/functional/array_transfer.hpp>

#include <xmipp4/core/dispatch/execution_context.hpp>
#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/array_view.hpp>
#include <xmipp4/core/ndarray/array_descriptor.hpp>
#include <xmipp4/core/layout/strided_layout.hpp>
#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/ndarray/operations/assignment/copy_operation.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/device_instance.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>

#include "../../core/dispatch/mock/mock_dispatcher.hpp"
#include "../../core/hardware/mock/mock_device.hpp"
#include "../../core/hardware/mock/mock_memory_resource.hpp"
#include "../../core/hardware/mock/mock_memory_allocator.hpp"
#include "../../core/hardware/mock/mock_command_queue.hpp"
#include "../../core/hardware/mock/mock_buffer.hpp"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <vector>

using namespace xmipp4;
using trompeloeil::_;

namespace
{

// Records the arguments seen by a mocked dispatch() call so that they can be
// inspected after the function under test has returned. The operation type is
// captured by value to stay valid past the operation's (temporary) lifetime.
struct dispatch_record
{
	bool called = false;
	const std::type_info *operation_type = nullptr;
	std::size_t num_outputs = 0;
	std::size_t num_inputs = 0;
	const buffer *first_output_storage = nullptr;
	const buffer *first_input_storage = nullptr;
	const xmipp4::device_instance *device_instance = nullptr;

	void operator()(
		const operation &op,
		span<array> outputs,
		span<const array_view> inputs,
		const device_context &device_context
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

class array_transfer_fixture
{
public:
	array_transfer_fixture()
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

		instance = std::make_shared<device_instance>(
			device,
			std::move(properties)
		);

		context = execution_context(
			device_context(instance),
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
	std::shared_ptr<const device_instance> instance;
	std::shared_ptr<mock_dispatcher> dispatcher;
	execution_context context;
};

} // namespace


//
// transfer
//

TEST_CASE(
	"transfer throws when the input array has no associated storage",
	"[array_transfer]"
)
{
	// A default-constructed array carries no storage.
	array input;

	// The context is irrelevant here: the storage check fails first.
	const execution_context context;

	CHECK_THROWS_AS(
		transfer(input, memory_resource_affinity::host, context),
		std::invalid_argument
	);
}

TEST_CASE(
	"transfer throws when the execution context has no allocator for the "
	"requested affinity",
	"[array_transfer]"
)
{
	const std::vector<std::size_t> extents = { 2, 3 };
	const array_descriptor descriptor(
		strided_layout::make_contiguous_layout(make_span(extents)),
		numerical_type::float32
	);
	const auto buffer = std::make_shared<mock_buffer>();
	array input(buffer, descriptor);

	// A default-constructed context is empty: it has no allocators.
	const execution_context context;

	CHECK_THROWS_AS(
		transfer(input, memory_resource_affinity::host, context),
		std::invalid_argument
	);
}

TEST_CASE_METHOD(
	array_transfer_fixture,
	"transfer aliases the input when it already lives on the target memory "
	"resource",
	"[array_transfer]"
)
{
	const auto descriptor = make_descriptor();
	const auto buffer = std::make_shared<mock_buffer>();

	// Both the input storage and the target allocator report the same memory
	// resource, so the input must be aliased verbatim: no allocation and no
	// copy dispatch are expected (an unexpected call would fail the test).
	ALLOW_CALL(*buffer, get_memory_resource())
		.LR_RETURN(host_resource);
	ALLOW_CALL(*host_allocator, get_memory_resource())
		.LR_RETURN(host_resource);

	array input(buffer, descriptor);

	const auto result = transfer(
		input,
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_storage() == buffer.get() );
	CHECK( result.get_descriptor() == descriptor );
}

TEST_CASE_METHOD(
	array_transfer_fixture,
	"transfer copies the input when it lives on a different memory resource",
	"[array_transfer]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto source_buffer = std::make_shared<mock_buffer>();
	const auto target_buffer = std::make_shared<mock_buffer>();

	// The input lives on the device resource while the host allocator hands out
	// host-resource storage, so aliasing is rejected and a copy is performed
	// via a fresh allocation on the host allocator.
	ALLOW_CALL(*source_buffer, get_memory_resource())
		.LR_RETURN(device_resource);
	ALLOW_CALL(*host_allocator, get_memory_resource())
		.LR_RETURN(host_resource);
	ALLOW_CALL(*host_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*host_allocator,
		allocate(size, _, default_queue.get())
	)
		.RETURN(target_buffer);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	array input(source_buffer, descriptor);

	const auto result = transfer(
		input,
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_storage() == target_buffer.get() );
	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(copy_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 1 );
	CHECK( record.first_output_storage == target_buffer.get() );
	CHECK( record.first_input_storage == source_buffer.get() );
	CHECK( record.device_instance == instance.get() );
}


//
// transfer_copy
//

TEST_CASE_METHOD(
	array_transfer_fixture,
	"transfer_copy allocates on the target allocator and dispatches a "
	"copy_operation with the input as the single source",
	"[array_transfer]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto source_buffer = std::make_shared<mock_buffer>();
	const auto target_buffer = std::make_shared<mock_buffer>();

	// A forced copy always allocates fresh storage on the target allocator.
	ALLOW_CALL(*host_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*host_allocator,
		allocate(size, _, default_queue.get())
	)
		.RETURN(target_buffer);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	array source(source_buffer, descriptor);

	const auto result = transfer_copy(
		source.share(),
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_storage() == target_buffer.get() );
	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(copy_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 1 );
	CHECK( record.first_output_storage == target_buffer.get() );
	CHECK( record.first_input_storage == source_buffer.get() );
	CHECK( record.device_instance == instance.get() );
}

TEST_CASE_METHOD(
	array_transfer_fixture,
	"transfer_copy reuses the storage of the provided output array",
	"[array_transfer]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto source_buffer = std::make_shared<mock_buffer>();
	const auto out_buffer = std::make_shared<mock_buffer>();

	// The output buffer lives on the target resource and is large enough, so it
	// must be reused verbatim: no allocate() call is expected.
	ALLOW_CALL(*out_buffer, get_memory_resource())
		.LR_RETURN(host_resource);
	ALLOW_CALL(*out_buffer, get_size())
		.RETURN(size);
	ALLOW_CALL(*host_allocator, get_memory_resource())
		.LR_RETURN(host_resource);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	array source(source_buffer, descriptor);
	array out(out_buffer, descriptor);

	const auto result = transfer_copy(
		source.share(),
		memory_resource_affinity::host,
		context,
		&out
	);

	// The output storage is reused and the copy dispatched into it.
	CHECK( result.get_storage() == out_buffer.get() );
	CHECK( out.get_storage() == out_buffer.get() );
	CHECK( record.called );
	CHECK( record.first_output_storage == out_buffer.get() );
	CHECK( record.first_input_storage == source_buffer.get() );
}


//
// to_device / to_host / to_device_copy / to_host_copy wrappers
//

TEST_CASE_METHOD(
	array_transfer_fixture,
	"to_device transfers using the device affinity",
	"[array_transfer]"
)
{
	const auto descriptor = make_descriptor();
	const auto buffer = std::make_shared<mock_buffer>();

	// The input lives on the device resource. Aliasing therefore proves that
	// to_device consulted the device allocator (a host allocator would report a
	// different resource and force a copy instead).
	ALLOW_CALL(*buffer, get_memory_resource())
		.LR_RETURN(device_resource);
	ALLOW_CALL(*device_allocator, get_memory_resource())
		.LR_RETURN(device_resource);

	array input(buffer, descriptor);

	const auto result = to_device(input, context);

	CHECK( result.get_storage() == buffer.get() );
}

TEST_CASE_METHOD(
	array_transfer_fixture,
	"to_host transfers using the host affinity",
	"[array_transfer]"
)
{
	const auto descriptor = make_descriptor();
	const auto buffer = std::make_shared<mock_buffer>();

	// Aliasing against the host resource proves the host allocator was used.
	ALLOW_CALL(*buffer, get_memory_resource())
		.LR_RETURN(host_resource);
	ALLOW_CALL(*host_allocator, get_memory_resource())
		.LR_RETURN(host_resource);

	array input(buffer, descriptor);

	const auto result = to_host(input, context);

	CHECK( result.get_storage() == buffer.get() );
}

TEST_CASE_METHOD(
	array_transfer_fixture,
	"to_device_copy allocates on the device allocator",
	"[array_transfer]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto source_buffer = std::make_shared<mock_buffer>();
	const auto device_buffer = std::make_shared<mock_buffer>();

	// Only the device allocator is expected to be used.
	ALLOW_CALL(*device_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*device_allocator,
		allocate(size, _, default_queue.get())
	)
		.RETURN(device_buffer);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	array source(source_buffer, descriptor);

	const auto result = to_device_copy(source.share(), context);

	CHECK( result.get_storage() == device_buffer.get() );
	CHECK( record.called );
	CHECK( *record.operation_type == typeid(copy_operation) );
	CHECK( record.first_input_storage == source_buffer.get() );
}

TEST_CASE_METHOD(
	array_transfer_fixture,
	"to_host_copy allocates on the host allocator",
	"[array_transfer]"
)
{
	const auto descriptor = make_descriptor();
	const auto size = compute_storage_requirement(descriptor);
	const auto source_buffer = std::make_shared<mock_buffer>();
	const auto host_buffer = std::make_shared<mock_buffer>();

	// Only the host allocator is expected to be used.
	ALLOW_CALL(*host_allocator, get_max_alignment())
		.RETURN(std::size_t(256));
	REQUIRE_CALL(
		*host_allocator,
		allocate(size, _, default_queue.get())
	)
		.RETURN(host_buffer);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	array source(source_buffer, descriptor);

	const auto result = to_host_copy(source.share(), context);

	CHECK( result.get_storage() == host_buffer.get() );
	CHECK( record.called );
	CHECK( *record.operation_type == typeid(copy_operation) );
	CHECK( record.first_input_storage == source_buffer.get() );
}
