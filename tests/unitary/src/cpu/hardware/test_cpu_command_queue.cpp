// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <cpu/hardware/cpu_command_queue.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/span.hpp>

#include "mock/mock_cpu_command.hpp"
#include "../../core/hardware/mock/mock_buffer.hpp"
#include "../../core/hardware/mock/mock_command.hpp"
#include "../../core/hardware/mock/mock_event.hpp"

#include <trompeloeil.hpp>

#include <memory>
#include <typeinfo>
#include <utility>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE(
	"cpu_command_queue::submit should forward execute to the cpu_command with "
	"host pointers of the operands",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_cpu_command cmd;

	int out0_data = 0;
	int out1_data = 0;
	int in0_data = 0;
	int in1_data = 0;

	auto out0 = std::make_shared<mock_buffer>();
	auto out1 = std::make_shared<mock_buffer>();
	auto in0 = std::make_shared<mock_buffer>();
	auto in1 = std::make_shared<mock_buffer>();

	REQUIRE_CALL(*out0, get_host_ptr()).LR_RETURN(&out0_data);
	REQUIRE_CALL(*out1, get_host_ptr()).LR_RETURN(&out1_data);
	REQUIRE_CALL(std::as_const(*in0), get_host_ptr()).LR_RETURN(&in0_data);
	REQUIRE_CALL(std::as_const(*in1), get_host_ptr()).LR_RETURN(&in1_data);

	REQUIRE_CALL(cmd, execute(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.WITH(_1.size() == 2)
		.LR_WITH(_1[0] == &out0_data && _1[1] == &out1_data)
		.WITH(_2.size() == 2)
		.LR_WITH(_2[0] == &in0_data && _2[1] == &in1_data)
		.WITH(_3.empty());

	const std::vector<std::shared_ptr<buffer>> outputs = { out0, out1 };
	const std::vector<std::shared_ptr<const buffer>> inputs = { in0, in1 };
	const std::vector<std::shared_ptr<buffer>> scratch;

	queue.submit(
		cmd, 
		make_span(outputs), 
		make_span(inputs), 
		make_span(scratch)
	);
}

TEST_CASE(
	"cpu_command_queue::submit should forward the scratch host pointers when "
	"scratch buffers are provided",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_cpu_command cmd;

	int scratch0_data = 0;
	int scratch1_data = 0;
	auto scratch0 = std::make_shared<mock_buffer>();
	auto scratch1 = std::make_shared<mock_buffer>();

	REQUIRE_CALL(*scratch0, get_host_ptr()).LR_RETURN(&scratch0_data);
	REQUIRE_CALL(*scratch1, get_host_ptr()).LR_RETURN(&scratch1_data);

	REQUIRE_CALL(cmd, execute(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.WITH(_1.empty())
		.WITH(_2.empty())
		.WITH(_3.size() == 2)
		.LR_WITH(_3[0] == &scratch0_data && _3[1] == &scratch1_data);

	const std::vector<std::shared_ptr<buffer>> outputs;
	const std::vector<std::shared_ptr<const buffer>> inputs;
	const std::vector<std::shared_ptr<buffer>> scratch = { scratch0, scratch1 };

	queue.submit(
		cmd, 
		make_span(outputs), 
		make_span(inputs), 
		make_span(scratch)
	);
}

TEST_CASE(
	"cpu_command_queue::submit should accept empty operand and scratch spans",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_cpu_command cmd;

	REQUIRE_CALL(cmd, execute(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.WITH(_1.empty())
		.WITH(_2.empty())
		.WITH(_3.empty());

	const std::vector<std::shared_ptr<buffer>> outputs;
	const std::vector<std::shared_ptr<const buffer>> inputs;
	const std::vector<std::shared_ptr<buffer>> scratch;

	queue.submit(
		cmd, make_span(outputs), make_span(inputs), make_span(scratch)
	);
}

TEST_CASE(
	"cpu_command_queue::submit should throw invalid_operation_error if an "
	"output operand is not host-accessible",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_cpu_command cmd;

	auto out = std::make_shared<mock_buffer>();
	REQUIRE_CALL(*out, get_host_ptr()).RETURN(nullptr);

	const std::vector<std::shared_ptr<buffer>> outputs = { out };
	const std::vector<std::shared_ptr<const buffer>> inputs;
	const std::vector<std::shared_ptr<buffer>> scratch;

	REQUIRE_THROWS_AS(
		queue.submit(
			cmd, 
			make_span(outputs), 
			make_span(inputs), 
			make_span(scratch)
		),
		invalid_operation_error
	);
}

TEST_CASE(
	"cpu_command_queue::submit should throw invalid_operation_error if an "
	"input operand is not host-accessible",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_cpu_command cmd;

	auto in = std::make_shared<mock_buffer>();
	REQUIRE_CALL(std::as_const(*in), get_host_ptr()).RETURN(nullptr);

	const std::vector<std::shared_ptr<buffer>> outputs;
	const std::vector<std::shared_ptr<const buffer>> inputs = { in };
	const std::vector<std::shared_ptr<buffer>> scratch;

	REQUIRE_THROWS_AS(
		queue.submit(
			cmd, 
			make_span(outputs), 
			make_span(inputs), 
			make_span(scratch)
		),
		invalid_operation_error
	);
}

TEST_CASE(
	"cpu_command_queue::submit should throw invalid_operation_error if the "
	"scratch buffer is not host-accessible",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_cpu_command cmd;

	auto scratch_buffer = std::make_shared<mock_buffer>();
	REQUIRE_CALL(*scratch_buffer, get_host_ptr()).RETURN(nullptr);

	const std::vector<std::shared_ptr<buffer>> outputs;
	const std::vector<std::shared_ptr<const buffer>> inputs;
	const std::vector<std::shared_ptr<buffer>> scratch = { scratch_buffer };

	REQUIRE_THROWS_AS(
		queue.submit(
			cmd, 
			make_span(outputs), 
			make_span(inputs), 
			make_span(scratch)
		),
		invalid_operation_error
	);
}

TEST_CASE(
	"cpu_command_queue::submit should throw std::bad_cast if the command is "
	"not a cpu_command",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_command cmd;

	const std::vector<std::shared_ptr<buffer>> outputs;
	const std::vector<std::shared_ptr<const buffer>> inputs;
	const std::vector<std::shared_ptr<buffer>> scratch;

	REQUIRE_THROWS_AS(
		queue.submit(
			cmd, 
			make_span(outputs), 
			make_span(inputs), 
			make_span(scratch)
		),
		std::bad_cast
	);
}

TEST_CASE(
	"cpu_command_queue::signal should return without touching the event",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_event event;
	REQUIRE_NOTHROW( queue.signal(event) );
}

TEST_CASE(
	"cpu_command_queue::wait should return without touching the event",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_event event;
	REQUIRE_NOTHROW( queue.wait(event) );
}

TEST_CASE(
	"cpu_command_queue::wait_until_completed should return immediately",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	REQUIRE_NOTHROW( queue.wait_until_completed() );
}

TEST_CASE(
	"cpu_command_queue::is_idle should always return true",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	REQUIRE( queue.is_idle() );
}
