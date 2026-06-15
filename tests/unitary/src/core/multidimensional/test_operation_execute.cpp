// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>
#include <xmipp4/core/multidimensional/execution_context.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/span.hpp>

#include "mock/mock_operation.hpp"
#include "mock/mock_operation_dispatcher.hpp"
#include "../hardware/mock/mock_device.hpp"
#include "../hardware/mock/mock_memory_resource.hpp"
#include "../hardware/mock/mock_memory_allocator.hpp"
#include "../hardware/mock/mock_command_queue.hpp"

#include <array>
#include <memory>
#include <vector>
#include <stdexcept>

using namespace xmipp4;
using namespace xmipp4::hardware;
using namespace xmipp4::multidimensional;

namespace
{

/**
 * @brief Fixture that wires a mock dispatcher, a mock device and a real
 * device_context together so that @ref execute can be driven end to end.
 *
 * The execution_context returned by @ref build_context is fully populated: it
 * owns the mock dispatcher, a device_context backed by a mock_device and the
 * mock device's default queue as its active queue. @ref expect_dispatch arms a
 * single dispatch() expectation and captures every argument forwarded to it so
 * that the test can assert what execute() passed down.
 */
class execute_fixture
{
public:
	execute_fixture()
		: device(std::make_shared<mock_device>())
		, host_allocator(std::make_shared<mock_memory_allocator>())
		, device_allocator(std::make_shared<mock_memory_allocator>())
		, default_queue(std::make_shared<mock_command_queue>())
		, dispatcher(std::make_shared<mock_operation_dispatcher>())
	{
	}

	execution_context build_context()
	{
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(
				*device,
				get_memory_resource(memory_resource_affinity::host)
			)
			.LR_RETURN(host_resource)
		);
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(
				*device,
				get_memory_resource(memory_resource_affinity::device)
			)
			.LR_RETURN(device_resource)
		);
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(host_resource, create_allocator())
			.RETURN(host_allocator)
		);
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(device_resource, create_allocator())
			.RETURN(device_allocator)
		);
		device_ctx = std::make_shared<device_context>(device);

		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(*device, get_default_queue())
			.RETURN(default_queue)
		);
		return execution_context(device_ctx, dispatcher);
	}

	void expect_dispatch()
	{
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(
				*dispatcher,
				dispatch(
					ANY(const operation&),
					ANY(span<array>),
					ANY(span<const array_view>),
					ANY(const hardware::device_context&),
					ANY(hardware::command_queue&)
				)
			)
			.LR_SIDE_EFFECT(captured_operation = &_1)
			.LR_SIDE_EFFECT(captured_output = _2)
			.LR_SIDE_EFFECT(captured_input = _3)
			.LR_SIDE_EFFECT(captured_device_context = &_4)
			.LR_SIDE_EFFECT(captured_queue = &_5)
		);
	}

protected:
	std::shared_ptr<mock_device> device;
	std::shared_ptr<memory_allocator> host_allocator;
	std::shared_ptr<memory_allocator> device_allocator;
	mock_memory_resource host_resource;
	mock_memory_resource device_resource;
	std::shared_ptr<command_queue> default_queue;
	std::shared_ptr<mock_operation_dispatcher> dispatcher;
	std::shared_ptr<const device_context> device_ctx;

	// Arguments captured by expect_dispatch().
	const operation *captured_operation = nullptr;
	span<array> captured_output;
	span<const array_view> captured_input;
	const device_context *captured_device_context = nullptr;
	const command_queue *captured_queue = nullptr;

private:
	std::vector<std::unique_ptr<trompeloeil::expectation>> m_expectations;
};

} // namespace



TEST_CASE(
	"execute throws when the context has no dispatcher",
	"[operation_execute]"
)
{
	const mock_operation op;
	std::array<array, 1> outputs;
	std::array<array_view, 1> inputs;

	// A default constructed context has no dispatcher, no queue and no device.
	const execution_context context;

	CHECK_THROWS_AS(
		execute(op, make_span(outputs), make_span(inputs), context),
		std::invalid_argument
	);
}

TEST_CASE_METHOD(
	execute_fixture,
	"execute throws when the context has no active queue",
	"[operation_execute]"
)
{
	const mock_operation op;
	std::array<array, 1> outputs;
	std::array<array_view, 1> inputs;

	// A context with a dispatcher but no device has no active queue either.
	const execution_context context =
		execution_context().with_dispatcher(dispatcher);

	CHECK_THROWS_AS(
		execute(op, make_span(outputs), make_span(inputs), context),
		std::invalid_argument
	);
}

TEST_CASE_METHOD(
	execute_fixture,
	"execute throws when the context has no device context",
	"[operation_execute]"
)
{
	const mock_operation op;
	std::array<array, 1> outputs;
	std::array<array_view, 1> inputs;

	// Force an active queue without a device so that the device context check
	// is the first one to fail.
	const execution_context context =
		execution_context().with_dispatcher(dispatcher).on_queue(default_queue);

	CHECK_THROWS_AS(
		execute(op, make_span(outputs), make_span(inputs), context),
		std::invalid_argument
	);
}

TEST_CASE_METHOD(
	execute_fixture,
	"execute forwards the operation, operands and resources to the dispatcher",
	"[operation_execute]"
)
{
	const mock_operation op;
	std::array<array, 2> outputs;
	std::array<array_view, 3> inputs;

	const execution_context context = build_context();
	expect_dispatch();

	execute(op, make_span(outputs), make_span(inputs), context);

	CHECK( captured_operation == &op );
	CHECK( captured_output.data() == outputs.data() );
	CHECK( captured_output.size() == outputs.size() );
	CHECK( captured_input.data() == inputs.data() );
	CHECK( captured_input.size() == inputs.size() );
	CHECK( captured_device_context == device_ctx.get() );
	CHECK( captured_queue == default_queue.get() );
}

TEST_CASE_METHOD(
	execute_fixture,
	"execute (array-returning) dispatches a single freshly created output when "
	"no out is given",
	"[operation_execute]"
)
{
	const mock_operation op;
	std::array<array_view, 1> inputs;

	const execution_context context = build_context();
	expect_dispatch();

	execute(op, make_span(inputs), context);

	CHECK( captured_operation == &op );
	CHECK( captured_output.size() == 1 );
	CHECK( captured_input.data() == inputs.data() );
	CHECK( captured_input.size() == inputs.size() );
	CHECK( captured_device_context == device_ctx.get() );
	CHECK( captured_queue == default_queue.get() );
}

TEST_CASE_METHOD(
	execute_fixture,
	"execute (array-returning) dispatches the provided out as the single output",
	"[operation_execute]"
)
{
	const mock_operation op;
	std::array<array_view, 1> inputs;
	array out;

	const execution_context context = build_context();
	expect_dispatch();

	execute(op, make_span(inputs), context, &out);

	// The provided output must be the one handed to the dispatcher.
	CHECK( captured_output.data() == &out );
	CHECK( captured_output.size() == 1 );
}

TEST_CASE_METHOD(
	execute_fixture,
	"execute_unary wraps the input into a single-element span",
	"[operation_execute]"
)
{
	const mock_operation op;
	const array_view input;

	const execution_context context = build_context();
	expect_dispatch();

	execute_unary(op, input, context);

	CHECK( captured_operation == &op );
	CHECK( captured_output.size() == 1 );
	CHECK( captured_input.data() == &input );
	CHECK( captured_input.size() == 1 );
}

TEST_CASE_METHOD(
	execute_fixture,
	"execute_binary wraps the inputs into a two-element span",
	"[operation_execute]"
)
{
	const mock_operation op;
	array_view first;
	array_view second;

	const execution_context context = build_context();
	expect_dispatch();

	execute_binary(op, std::move(first), std::move(second), context);

	CHECK( captured_operation == &op );
	CHECK( captured_output.size() == 1 );
	CHECK( captured_input.size() == 2 );
}

TEST_CASE_METHOD(
	execute_fixture,
	"execute_ternary wraps the inputs into a three-element span",
	"[operation_execute]"
)
{
	const mock_operation op;
	array_view first;
	array_view second;
	array_view third;

	const execution_context context = build_context();
	expect_dispatch();

	execute_ternary(
		op,
		std::move(first),
		std::move(second),
		std::move(third),
		context
	);

	CHECK( captured_operation == &op );
	CHECK( captured_output.size() == 1 );
	CHECK( captured_input.size() == 3 );
}
