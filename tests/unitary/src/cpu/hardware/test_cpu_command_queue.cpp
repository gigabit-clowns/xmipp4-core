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

	REQUIRE_CALL(*out0, get_host_ptr()).RETURN(&out0_data);
	REQUIRE_CALL(*out1, get_host_ptr()).RETURN(&out1_data);
	REQUIRE_CALL(std::as_const(*in0), get_host_ptr()).RETURN(&in0_data);
	REQUIRE_CALL(std::as_const(*in1), get_host_ptr()).RETURN(&in1_data);

	REQUIRE_CALL(cmd, execute(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.WITH(_1.size() == 2)
		.WITH(_1[0] == &out0_data && _1[1] == &out1_data)
		.WITH(_2.size() == 2)
		.WITH(_2[0] == &in0_data && _2[1] == &in1_data)
		.WITH(_3 == nullptr);

	const std::vector<std::shared_ptr<buffer>> outputs = { out0, out1 };
	const std::vector<std::shared_ptr<const buffer>> inputs = { in0, in1 };

	queue.submit(cmd, make_span(outputs), make_span(inputs));
}

TEST_CASE(
	"cpu_command_queue::submit should forward the scratch host pointer when "
	"a scratch buffer is provided",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_cpu_command cmd;

	int scratch_data = 0;
	auto scratch = std::make_shared<mock_buffer>();

	REQUIRE_CALL(*scratch, get_host_ptr()).RETURN(&scratch_data);

	REQUIRE_CALL(cmd, execute(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.WITH(_1.empty())
		.WITH(_2.empty())
		.WITH(_3 == &scratch_data);

	const std::vector<std::shared_ptr<buffer>> outputs;
	const std::vector<std::shared_ptr<const buffer>> inputs;

	queue.submit(cmd, make_span(outputs), make_span(inputs), scratch);
}

TEST_CASE(
	"cpu_command_queue::submit should accept empty operand spans and a null "
	"scratch buffer",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	mock_cpu_command cmd;

	REQUIRE_CALL(cmd, execute(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.WITH(_1.empty())
		.WITH(_2.empty())
		.WITH(_3 == nullptr);

	const std::vector<std::shared_ptr<buffer>> outputs;
	const std::vector<std::shared_ptr<const buffer>> inputs;

	queue.submit(cmd, make_span(outputs), make_span(inputs));
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

	REQUIRE_THROWS_AS(
		queue.submit(cmd, make_span(outputs), make_span(inputs)),
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

	REQUIRE_THROWS_AS(
		queue.submit(cmd, make_span(outputs), make_span(inputs)),
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

	auto scratch = std::make_shared<mock_buffer>();
	REQUIRE_CALL(*scratch, get_host_ptr()).RETURN(nullptr);

	const std::vector<std::shared_ptr<buffer>> outputs;
	const std::vector<std::shared_ptr<const buffer>> inputs;

	REQUIRE_THROWS_AS(
		queue.submit(cmd, make_span(outputs), make_span(inputs), scratch),
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

	REQUIRE_THROWS_AS(
		queue.submit(cmd, make_span(outputs), make_span(inputs)),
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
