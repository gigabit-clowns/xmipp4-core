// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <backends/cpu/builders/assignment/fill_program_builder.hpp>
#include <backends/cpu/hardware/command_queue.hpp>
#include <core/hardware/host_memory/host_buffer.hpp>

#include <xmipp4/backends/cpu/program.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/ops/assignment/fill_operation.hpp>
#include <xmipp4/core/dispatch/operand_signature.hpp>
#include <xmipp4/core/layout/strided_layout.hpp>
#include <xmipp4/core/dispatch/operation_id.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>
#include <xmipp4/core/span.hpp>

#include "../../../../core/dispatch/mock/mock_operation.hpp"

#include <complex>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::cpu;
using namespace xmipp4::ops;

namespace
{

operand_signature make_contiguous_signature(
	std::initializer_list<std::size_t> extents,
	numerical_type type
)
{
	const std::vector<std::size_t> ext(extents);
	return operand_signature(
		strided_layout::make_contiguous_layout(make_span(ext)),
		type
	);
}

operand_signature make_strided_signature(
	std::initializer_list<std::size_t> extents,
	std::initializer_list<std::ptrdiff_t> strides,
	numerical_type type
)
{
	const std::vector<std::size_t> ext(extents);
	const std::vector<std::ptrdiff_t> str(strides);
	return operand_signature(
		strided_layout::make_custom_layout(make_span(ext), make_span(str)),
		type
	);
}

class cpu_fill_program_builder_fixture
{
public:
	// Build the program for the given operation and downcast it to the CPU
	// program interface that exposes execute().
	std::shared_ptr<cpu::program> build_program(
		const operation &op,
		const std::vector<operand_signature> &outputs,
		const std::vector<operand_signature> &inputs
	)
	{
		auto program = builder.build(
			op,
			make_span(outputs),
			make_span(inputs),
			queue,
			nullptr
		);

		auto cpu_program = std::dynamic_pointer_cast<cpu::program>(program);
		REQUIRE( cpu_program != nullptr );
		return cpu_program;
	}

	// Execute a fill program. Fill declares neither input nor scratch operands.
	void execute(
		const cpu::program &program,
		std::shared_ptr<buffer> destination
	)
	{
		const std::vector<std::shared_ptr<buffer>> outputs {
			std::move(destination)
		};
		const std::vector<std::shared_ptr<const buffer>> inputs;
		const std::vector<std::shared_ptr<buffer>> scratch;

		program.execute(
			make_span(outputs),
			make_span(inputs),
			make_span(scratch)
		);
	}

protected:
	fill_program_builder builder;
	cpu::command_queue queue;
};

} // namespace


TEST_CASE(
	"fill_program_builder reports the fill_operation id",
	"[fill_program_builder]"
)
{
	const fill_program_builder builder;

	CHECK( builder.get_operation_id() == operation_id::of<fill_operation>() );
}

TEST_CASE_METHOD(
	cpu_fill_program_builder_fixture,
	"fill_program_builder::build throws when the operation is not "
	"a fill_operation",
	"[fill_program_builder]"
)
{
	const mock_operation operation;

	const std::vector<operand_signature> outputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};
	const std::vector<operand_signature> inputs;

	CHECK_THROWS_AS(
		builder.build(
			operation,
			make_span(outputs),
			make_span(inputs),
			queue,
			nullptr
		),
		std::invalid_argument
	);
}

TEST_CASE_METHOD(
	cpu_fill_program_builder_fixture,
	"fill_program_builder::build throws when the output signature "
	"count is not one",
	"[fill_program_builder]"
)
{
	const fill_operation operation(scalar_value(1.0f));

	// No output signatures where exactly one is expected.
	const std::vector<operand_signature> outputs;
	const std::vector<operand_signature> inputs;

	CHECK_THROWS_AS(
		builder.build(
			operation,
			make_span(outputs),
			make_span(inputs),
			queue,
			nullptr
		),
		std::invalid_argument
	);
}

TEST_CASE_METHOD(
	cpu_fill_program_builder_fixture,
	"fill_program_builder::build throws when input signatures are "
	"provided",
	"[fill_program_builder]"
)
{
	const fill_operation operation(scalar_value(1.0f));

	const std::vector<operand_signature> outputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};
	// Fill takes no inputs, so any input signature is rejected.
	const std::vector<operand_signature> inputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};

	CHECK_THROWS_AS(
		builder.build(
			operation,
			make_span(outputs),
			make_span(inputs),
			queue,
			nullptr
		),
		std::invalid_argument
	);
}

TEST_CASE_METHOD(
	cpu_fill_program_builder_fixture,
	"fill_program_builder builds a program that fills a contiguous "
	"array with the given value",
	"[fill_program_builder]"
)
{
	const fill_operation operation(scalar_value(2.5f));

	const std::vector<operand_signature> outputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};
	const std::vector<operand_signature> inputs;

	const auto program = build_program(operation, outputs, inputs);

	const auto destination =
		std::make_shared<host_buffer>(4 * sizeof(float), 64);
	auto *dst_ptr = static_cast<float*>(destination->get_host_ptr());
	for (std::size_t i = 0; i < 4; ++i)
	{
		dst_ptr[i] = 0.0f;
	}

	execute(*program, destination);

	for (std::size_t i = 0; i < 4; ++i)
	{
		CHECK( dst_ptr[i] == 2.5f );
	}
}

TEST_CASE_METHOD(
	cpu_fill_program_builder_fixture,
	"fill_program_builder builds a program that casts the fill "
	"value into the destination type",
	"[fill_program_builder]"
)
{
	// The fill value is an integer while the destination is float32.
	const fill_operation operation(scalar_value(std::int32_t(7)));

	const std::vector<operand_signature> outputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};
	const std::vector<operand_signature> inputs;

	const auto program = build_program(operation, outputs, inputs);

	const auto destination =
		std::make_shared<host_buffer>(4 * sizeof(float), 64);
	auto *dst_ptr = static_cast<float*>(destination->get_host_ptr());
	for (std::size_t i = 0; i < 4; ++i)
	{
		dst_ptr[i] = 0.0f;
	}

	execute(*program, destination);

	for (std::size_t i = 0; i < 4; ++i)
	{
		CHECK( dst_ptr[i] == 7.0f );
	}
}

TEST_CASE_METHOD(
	cpu_fill_program_builder_fixture,
	"fill_program_builder builds a program that honours a strided "
	"destination and leaves the gaps untouched",
	"[fill_program_builder]"
)
{
	const fill_operation operation(scalar_value(9.0f));

	// Destination writes to elements 0, 2 and 4 of a 5-element buffer.
	const std::vector<operand_signature> outputs {
		make_strided_signature({3}, {2}, numerical_type::float32)
	};
	const std::vector<operand_signature> inputs;

	const auto program = build_program(operation, outputs, inputs);

	const auto destination =
		std::make_shared<host_buffer>(5 * sizeof(float), 64);
	auto *dst_ptr = static_cast<float*>(destination->get_host_ptr());
	for (std::size_t i = 0; i < 5; ++i)
	{
		dst_ptr[i] = -1.0f;
	}

	execute(*program, destination);

	CHECK( dst_ptr[0] == 9.0f );
	CHECK( dst_ptr[2] == 9.0f );
	CHECK( dst_ptr[4] == 9.0f );

	// The elements skipped by the stride keep their original value.
	CHECK( dst_ptr[1] == -1.0f );
	CHECK( dst_ptr[3] == -1.0f );
}

TEST_CASE_METHOD(
	cpu_fill_program_builder_fixture,
	"fill_program_builder::build throws when the fill value type "
	"can not be converted into the destination type",
	"[fill_program_builder]"
)
{
	// A complex fill value can not be narrowed into a real destination.
	const fill_operation operation(
		scalar_value(std::complex<float>(1.0f, 2.0f))
	);

	const std::vector<operand_signature> outputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};
	const std::vector<operand_signature> inputs;

	CHECK_THROWS_AS(
		builder.build(
			operation,
			make_span(outputs),
			make_span(inputs),
			queue,
			nullptr
		),
		std::invalid_argument
	);
}
