// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <cpu/hardware/cpu_command_queue.hpp>

#include <xmipp4/core/hardware/command.hpp>
#include <xmipp4/core/span.hpp>

#include "mock/mock_cpu_program.hpp"
#include "../../core/hardware/mock/mock_buffer.hpp"
#include "../../core/hardware/mock/mock_program.hpp"
#include "../../core/hardware/mock/mock_event.hpp"

#include <memory>
#include <typeinfo>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE(
	"cpu_command_queue::create() should return always the same instance",
	"[cpu_command_queue]"
)
{
	const auto q1 = cpu_command_queue::create();
	const auto q2 = cpu_command_queue::create();
	CHECK( q1 == q2 );
}

TEST_CASE(
	"cpu_command_queue::submit throws when the command has no program",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	const command cmd;
	CHECK_THROWS_AS( queue.submit(cmd), std::invalid_argument );
}

TEST_CASE(
	"cpu_command_queue::submit throws when the program is not a cpu_program",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;
	const command cmd(std::make_shared<mock_program>());
	CHECK_THROWS_AS( queue.submit(cmd), std::bad_cast );
}

TEST_CASE(
	"cpu_command_queue::submit calls execute with the bound operands",
	"[cpu_command_queue]"
)
{
	cpu_command_queue queue;

	const auto prog = std::make_shared<mock_cpu_program>();
	const std::vector<std::shared_ptr<buffer>> outputs = { 
		std::make_shared<mock_buffer>(),
		std::make_shared<mock_buffer>(),
		std::make_shared<mock_buffer>() 
	};
	const std::vector<std::shared_ptr<const buffer>> inputs = { 
		std::make_shared<mock_buffer>(),
		std::make_shared<mock_buffer>(),
		std::make_shared<mock_buffer>(),
		std::make_shared<mock_buffer>() 
	};
	const std::vector<std::shared_ptr<buffer>> scratch = { 
		std::make_shared<mock_buffer>(),
		std::make_shared<mock_buffer>() 
	};

	command cmd(prog);
	cmd.bind_outputs(make_span(outputs))
	   .bind_inputs(make_span(inputs))
	   .bind_scratch(make_span(scratch));

	REQUIRE_CALL(*prog, execute(trompeloeil::_, trompeloeil::_, trompeloeil::_))
		.LR_WITH(_1.data() == outputs.data() && _1.size() == outputs.size())
		.LR_WITH(_2.data() == inputs.data() && _2.size() == inputs.size())
		.LR_WITH(_3.data() == scratch.data() && _3.size() == scratch.size());

	REQUIRE_NOTHROW( queue.submit(cmd) );
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
