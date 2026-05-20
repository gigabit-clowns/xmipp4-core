// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <trompeloeil.hpp>

#include <cpu/hardware/cpu_command_queue.hpp>
#include <cpu/hardware/cpu_event.hpp>

#include <xmipp4/cpu/hardware/cpu_command.hpp>
#include <xmipp4/core/hardware/event.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/span.hpp>

#include "../../core/hardware/mock/mock_buffer.hpp"

#include <memory>
#include <typeinfo>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::hardware;

namespace
{

class mock_event
	: public event
{
public:
	MAKE_CONST_MOCK0(
		get_supported_usage,
		event_usage_flags(),
		noexcept override
	);
	MAKE_CONST_MOCK0(wait, void(), override);
	MAKE_CONST_MOCK0(is_signaled, bool(), override);
};

class recording_cpu_command final
	: public cpu_command
{
public:
	mutable std::vector<void*> outputs;
	mutable std::vector<const void*> inputs;
	mutable std::size_t call_count = 0;

	void execute(
		span<void* const> out,
		span<const void* const> in
	) const override
	{
		outputs.assign(out.begin(), out.end());
		inputs.assign(in.begin(), in.end());
		++call_count;
	}
};

class non_cpu_command final
	: public command
{
};

} // anonymous namespace

TEST_CASE( "cpu_command_queue is always idle", "[cpu_command_queue]" )
{
	cpu_command_queue queue;
	REQUIRE( queue.is_idle() );
}

TEST_CASE( "cpu_command_queue::wait_until_completed is a no-op", "[cpu_command_queue]" )
{
	cpu_command_queue queue;
	REQUIRE_NOTHROW( queue.wait_until_completed() );
}

TEST_CASE( "cpu_command_queue::signal does not interact with the event", "[cpu_command_queue]" )
{
	cpu_command_queue queue;
	mock_event evt;

	REQUIRE_NOTHROW( queue.signal(evt) );
}

TEST_CASE( "cpu_command_queue::wait does not interact with the event", "[cpu_command_queue]" )
{
	cpu_command_queue queue;
	mock_event evt;

	REQUIRE_NOTHROW( queue.wait(evt) );
}

TEST_CASE( "cpu_command_queue::submit executes a cpu_command with empty operands", "[cpu_command_queue]" )
{
	cpu_command_queue queue;
	recording_cpu_command cmd;

	std::vector<std::shared_ptr<buffer>> outputs;
	std::vector<std::shared_ptr<const buffer>> inputs;

	queue.submit(cmd, make_span(outputs), make_span(inputs));

	REQUIRE( cmd.call_count == 1 );
	REQUIRE( cmd.outputs.empty() );
	REQUIRE( cmd.inputs.empty() );
}

TEST_CASE( "cpu_command_queue::submit forwards host pointers to the command", "[cpu_command_queue]" )
{
	cpu_command_queue queue;
	recording_cpu_command cmd;

	int out_storage_a = 0;
	int out_storage_b = 0;
	int in_storage = 0;

	auto out_buf_a = std::make_shared<mock_buffer>();
	auto out_buf_b = std::make_shared<mock_buffer>();
	auto in_buf = std::make_shared<mock_buffer>();

	REQUIRE_CALL(*out_buf_a, get_host_ptr())
		.RETURN(&out_storage_a);
	REQUIRE_CALL(*out_buf_b, get_host_ptr())
		.RETURN(&out_storage_b);
	REQUIRE_CALL(
		const_cast<const mock_buffer&>(*in_buf),
		get_host_ptr()
	).RETURN(&in_storage);

	std::vector<std::shared_ptr<buffer>> outputs = { out_buf_a, out_buf_b };
	std::vector<std::shared_ptr<const buffer>> inputs = { in_buf };

	queue.submit(cmd, make_span(outputs), make_span(inputs));

	REQUIRE( cmd.call_count == 1 );
	REQUIRE( cmd.outputs.size() == 2 );
	REQUIRE( cmd.outputs[0] == &out_storage_a );
	REQUIRE( cmd.outputs[1] == &out_storage_b );
	REQUIRE( cmd.inputs.size() == 1 );
	REQUIRE( cmd.inputs[0] == &in_storage );
}

TEST_CASE( "cpu_command_queue::submit throws bad_cast when the command is not a cpu_command", "[cpu_command_queue]" )
{
	cpu_command_queue queue;
	non_cpu_command cmd;

	std::vector<std::shared_ptr<buffer>> outputs;
	std::vector<std::shared_ptr<const buffer>> inputs;

	REQUIRE_THROWS_AS(
		queue.submit(cmd, make_span(outputs), make_span(inputs)),
		std::bad_cast
	);
}

TEST_CASE( "cpu_command_queue::submit throws when an output buffer is not host accessible", "[cpu_command_queue]" )
{
	cpu_command_queue queue;
	recording_cpu_command cmd;

	auto bad_buf = std::make_shared<mock_buffer>();
	ALLOW_CALL(*bad_buf, get_host_ptr())
		.RETURN(nullptr);

	std::vector<std::shared_ptr<buffer>> outputs = { bad_buf };
	std::vector<std::shared_ptr<const buffer>> inputs;

	REQUIRE_THROWS_MATCHES(
		queue.submit(cmd, make_span(outputs), make_span(inputs)),
		invalid_operation_error,
		Catch::Matchers::MessageMatches(
			Catch::Matchers::ContainsSubstring("output operand is not")
		)
	);
	REQUIRE( cmd.call_count == 0 );
}

TEST_CASE( "cpu_command_queue::submit throws when an input buffer is not host accessible", "[cpu_command_queue]" )
{
	cpu_command_queue queue;
	recording_cpu_command cmd;

	auto bad_buf = std::make_shared<mock_buffer>();
	ALLOW_CALL(
		const_cast<const mock_buffer&>(*bad_buf),
		get_host_ptr()
	).RETURN(nullptr);

	std::vector<std::shared_ptr<buffer>> outputs;
	std::vector<std::shared_ptr<const buffer>> inputs = { bad_buf };

	REQUIRE_THROWS_MATCHES(
		queue.submit(cmd, make_span(outputs), make_span(inputs)),
		invalid_operation_error,
		Catch::Matchers::MessageMatches(
			Catch::Matchers::ContainsSubstring("input operand is not")
		)
	);
	REQUIRE( cmd.call_count == 0 );
}
