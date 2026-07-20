// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/functional/arithmetic.hpp>

#include <xmipp4/core/dispatch/execution_context.hpp>
#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/const_array.hpp>
#include <xmipp4/core/ndarray/const_array_ref.hpp>
#include <xmipp4/core/ndarray/array_descriptor.hpp>
#include <xmipp4/core/layout/strided_layout.hpp>
#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/ops/arithmetic/add_operation.hpp>
#include <xmipp4/ops/arithmetic/subtract_operation.hpp>
#include <xmipp4/ops/arithmetic/negate_operation.hpp>
#include <xmipp4/ops/arithmetic/multiply_operation.hpp>
#include <xmipp4/ops/arithmetic/divide_operation.hpp>
#include <xmipp4/ops/arithmetic/modulo_operation.hpp>
#include <xmipp4/ops/arithmetic/abs_operation.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
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
#include <typeinfo>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::ops;
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
	const buffer *first_output_storage = nullptr;
	const buffer *first_input_storage = nullptr;
	const buffer *second_input_storage = nullptr;
	const xmipp4::device_session *device_session = nullptr;

	void operator()(
		const operation &op,
		span<array> outputs,
		span<const const_array_ref> inputs,
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
		if (inputs.size() > 1)
		{
			second_input_storage = inputs[1].get_storage();
		}
		device_session = device_context.get_device_session().get();
	}
};

class array_arithmetic_fixture
{
public:
	array_arithmetic_fixture()
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


TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"add dispatches an add_operation with both inputs and no pre-allocated "
	"output when out is null",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto y_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	array y_array(y_buffer, descriptor);
	const_array_ref x = x_array;
	const_array_ref y = y_array;

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	add(x, y, context, nullptr);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(add_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 2 );
	CHECK( record.first_output_storage == nullptr );
	CHECK( record.first_input_storage == x_buffer.get() );
	CHECK( record.second_input_storage == y_buffer.get() );
	CHECK( record.device_session == session.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"add dispatches an add_operation reusing the provided output array's "
	"storage",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto y_buffer = std::make_shared<mock_buffer>();
	const auto out_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	array y_array(y_buffer, descriptor);
	const_array_ref x = x_array;
	const_array_ref y = y_array;
	array out(out_buffer, descriptor);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = add(x, y, context, &out);

	CHECK( result.get_storage() == out_buffer.get() );
	CHECK( record.called );
	CHECK( *record.operation_type == typeid(add_operation) );
	CHECK( record.first_output_storage == out_buffer.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"subtract dispatches a subtract_operation with both inputs in order",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto y_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	array y_array(y_buffer, descriptor);
	const_array_ref x = x_array;
	const_array_ref y = y_array;

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	subtract(x, y, context, nullptr);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(subtract_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 2 );
	CHECK( record.first_input_storage == x_buffer.get() );
	CHECK( record.second_input_storage == y_buffer.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"subtract dispatches a subtract_operation reusing the provided output "
	"array's storage",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto y_buffer = std::make_shared<mock_buffer>();
	const auto out_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	array y_array(y_buffer, descriptor);
	const_array_ref x = x_array;
	const_array_ref y = y_array;
	array out(out_buffer, descriptor);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = subtract(x, y, context, &out);

	CHECK( result.get_storage() == out_buffer.get() );
	CHECK( record.called );
	CHECK( *record.operation_type == typeid(subtract_operation) );
	CHECK( record.first_output_storage == out_buffer.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"multiply dispatches a multiply_operation with both inputs in order",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto y_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	array y_array(y_buffer, descriptor);
	const_array_ref x = x_array;
	const_array_ref y = y_array;

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	multiply(x, y, context, nullptr);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(multiply_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 2 );
	CHECK( record.first_input_storage == x_buffer.get() );
	CHECK( record.second_input_storage == y_buffer.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"multiply dispatches a multiply_operation reusing the provided output "
	"array's storage",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto y_buffer = std::make_shared<mock_buffer>();
	const auto out_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	array y_array(y_buffer, descriptor);
	const_array_ref x = x_array;
	const_array_ref y = y_array;
	array out(out_buffer, descriptor);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = multiply(x, y, context, &out);

	CHECK( result.get_storage() == out_buffer.get() );
	CHECK( record.called );
	CHECK( *record.operation_type == typeid(multiply_operation) );
	CHECK( record.first_output_storage == out_buffer.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"divide dispatches a divide_operation with both inputs in order",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto y_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	array y_array(y_buffer, descriptor);
	const_array_ref x = x_array;
	const_array_ref y = y_array;

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	divide(x, y, context, nullptr);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(divide_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 2 );
	CHECK( record.first_input_storage == x_buffer.get() );
	CHECK( record.second_input_storage == y_buffer.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"divide dispatches a divide_operation reusing the provided output "
	"array's storage",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto y_buffer = std::make_shared<mock_buffer>();
	const auto out_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	array y_array(y_buffer, descriptor);
	const_array_ref x = x_array;
	const_array_ref y = y_array;
	array out(out_buffer, descriptor);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = divide(x, y, context, &out);

	CHECK( result.get_storage() == out_buffer.get() );
	CHECK( record.called );
	CHECK( *record.operation_type == typeid(divide_operation) );
	CHECK( record.first_output_storage == out_buffer.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"modulo dispatches a modulo_operation with both inputs and no "
	"pre-allocated output when out is null",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto y_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	array y_array(y_buffer, descriptor);
	const_array_ref x = x_array;
	const_array_ref y = y_array;

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	modulo(x, y, context, nullptr);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(modulo_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 2 );
	CHECK( record.first_output_storage == nullptr );
	CHECK( record.first_input_storage == x_buffer.get() );
	CHECK( record.second_input_storage == y_buffer.get() );
	CHECK( record.device_session == session.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"modulo dispatches a modulo_operation reusing the provided output "
	"array's storage",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto y_buffer = std::make_shared<mock_buffer>();
	const auto out_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	array y_array(y_buffer, descriptor);
	const_array_ref x = x_array;
	const_array_ref y = y_array;
	array out(out_buffer, descriptor);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = modulo(x, y, context, &out);

	CHECK( result.get_storage() == out_buffer.get() );
	CHECK( record.called );
	CHECK( *record.operation_type == typeid(modulo_operation) );
	CHECK( record.first_output_storage == out_buffer.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"negate dispatches a negate_operation with a single input and no "
	"pre-allocated output when out is null",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	const_array_ref x = x_array;

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	negate(x, context, nullptr);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(negate_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 1 );
	CHECK( record.first_output_storage == nullptr );
	CHECK( record.first_input_storage == x_buffer.get() );
	CHECK( record.device_session == session.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"negate dispatches a negate_operation reusing the provided output "
	"array's storage",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto out_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	const_array_ref x = x_array;
	array out(out_buffer, descriptor);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = negate(x, context, &out);

	CHECK( result.get_storage() == out_buffer.get() );
	CHECK( record.called );
	CHECK( *record.operation_type == typeid(negate_operation) );
	CHECK( record.first_output_storage == out_buffer.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"abs dispatches an abs_operation with a single input and no "
	"pre-allocated output when out is null",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	const_array_ref x = x_array;

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	abs(x, context, nullptr);

	CHECK( record.called );
	REQUIRE( record.operation_type != nullptr );
	CHECK( *record.operation_type == typeid(abs_operation) );
	CHECK( record.num_outputs == 1 );
	CHECK( record.num_inputs == 1 );
	CHECK( record.first_output_storage == nullptr );
	CHECK( record.first_input_storage == x_buffer.get() );
	CHECK( record.device_session == session.get() );
}

TEST_CASE_METHOD(
	array_arithmetic_fixture,
	"abs dispatches an abs_operation reusing the provided output array's "
	"storage",
	"[array_arithmetic]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });
	const auto x_buffer = std::make_shared<mock_buffer>();
	const auto out_buffer = std::make_shared<mock_buffer>();
	array x_array(x_buffer, descriptor);
	const_array_ref x = x_array;
	array out(out_buffer, descriptor);

	dispatch_record record;
	REQUIRE_CALL(*dispatcher, dispatch(_, _, _, _))
		.LR_SIDE_EFFECT( record(_1, _2, _3, _4) );

	const auto result = abs(x, context, &out);

	CHECK( result.get_storage() == out_buffer.get() );
	CHECK( record.called );
	CHECK( *record.operation_type == typeid(abs_operation) );
	CHECK( record.first_output_storage == out_buffer.get() );
}
