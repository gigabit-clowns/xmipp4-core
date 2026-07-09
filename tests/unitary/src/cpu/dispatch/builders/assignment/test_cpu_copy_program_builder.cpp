// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <cpu/dispatch/builders/assignment/cpu_copy_program_builder.hpp>
#include <cpu/hardware/cpu_command_queue.hpp>
#include <core/hardware/host_memory/host_buffer.hpp>

#include <xmipp4/cpu/hardware/cpu_program.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/operations/assignment/copy_operation.hpp>
#include <xmipp4/core/ndarray/array_signature.hpp>
#include <xmipp4/core/layout/strided_layout.hpp>
#include <xmipp4/core/dispatch/operation_id.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/span.hpp>

#include "../../../../core/dispatch/mock/mock_operation.hpp"

#include <complex>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::dispatch;
using namespace xmipp4::operations;
using namespace xmipp4::ndarray;
using namespace xmipp4::layout;

namespace
{

array_signature make_contiguous_signature(
	std::initializer_list<std::size_t> extents,
	numerical_type type
)
{
	const std::vector<std::size_t> ext(extents);
	return array_signature(
		strided_layout::make_contiguous_layout(make_span(ext)),
		type
	);
}

array_signature make_strided_signature(
	std::initializer_list<std::size_t> extents,
	std::initializer_list<std::ptrdiff_t> strides,
	numerical_type type
)
{
	const std::vector<std::size_t> ext(extents);
	const std::vector<std::ptrdiff_t> str(strides);
	return array_signature(
		strided_layout::make_custom_layout(make_span(ext), make_span(str)),
		type
	);
}

class cpu_copy_program_builder_fixture
{
public:
	// Build the program for the given operation and downcast it to the CPU
	// program interface that exposes execute().
	std::shared_ptr<hardware::cpu_program> build_program(
		const operation &op,
		const std::vector<array_signature> &outputs,
		const std::vector<array_signature> &inputs
	)
	{
		auto program = builder.build(
			op,
			make_span(outputs),
			make_span(inputs),
			queue,
			nullptr
		);

		auto cpu = std::dynamic_pointer_cast<hardware::cpu_program>(program);
		REQUIRE( cpu != nullptr );
		return cpu;
	}

	// Execute a copy program. Copy declares no scratch operands.
	void execute(
		const hardware::cpu_program &program,
		std::shared_ptr<hardware::buffer> destination,
		std::shared_ptr<const hardware::buffer> source
	)
	{
		const std::vector<std::shared_ptr<hardware::buffer>> outputs {
			std::move(destination)
		};
		const std::vector<std::shared_ptr<const hardware::buffer>> inputs {
			std::move(source)
		};
		const std::vector<std::shared_ptr<hardware::buffer>> scratch;

		program.execute(
			make_span(outputs),
			make_span(inputs),
			make_span(scratch)
		);
	}

protected:
	cpu_copy_program_builder builder;
	hardware::cpu_command_queue queue;
};

} // namespace



TEST_CASE(
	"cpu_copy_program_builder reports the copy_operation id",
	"[cpu_copy_program_builder]"
)
{
	const cpu_copy_program_builder builder;

	CHECK( builder.get_operation_id() == operation_id::of<copy_operation>() );
}

TEST_CASE_METHOD(
	cpu_copy_program_builder_fixture,
	"cpu_copy_program_builder::build throws when the operation is not "
	"a copy_operation",
	"[cpu_copy_program_builder]"
)
{
	const mock_operation operation;

	const std::vector<array_signature> outputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};
	const std::vector<array_signature> inputs {
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
	cpu_copy_program_builder_fixture,
	"cpu_copy_program_builder::build throws when the output signature "
	"count is not one",
	"[cpu_copy_program_builder]"
)
{
	const copy_operation operation;

	// No output signatures where exactly one is expected.
	const std::vector<array_signature> outputs;
	const std::vector<array_signature> inputs {
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
	cpu_copy_program_builder_fixture,
	"cpu_copy_program_builder::build throws when the input signature "
	"count is not one",
	"[cpu_copy_program_builder]"
)
{
	const copy_operation operation;

	const std::vector<array_signature> outputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};
	// No input signatures where exactly one is expected.
	const std::vector<array_signature> inputs;

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
	cpu_copy_program_builder_fixture,
	"cpu_copy_program_builder builds a program that copies a "
	"contiguous array element-wise",
	"[cpu_copy_program_builder]"
)
{
	const copy_operation operation;

	const std::vector<array_signature> outputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};
	const std::vector<array_signature> inputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};

	const auto program = build_program(operation, outputs, inputs);

	const auto destination =
		std::make_shared<hardware::host_buffer>(4 * sizeof(float), 64);
	const auto source =
		std::make_shared<hardware::host_buffer>(4 * sizeof(float), 64);
	auto *dst_ptr = static_cast<float*>(destination->get_host_ptr());
	auto *src_ptr = static_cast<float*>(source->get_host_ptr());
	for (std::size_t i = 0; i < 4; ++i)
	{
		dst_ptr[i] = 0.0f;
		src_ptr[i] = static_cast<float>(i + 1);
	}

	execute(*program, destination, source);

	for (std::size_t i = 0; i < 4; ++i)
	{
		CHECK( dst_ptr[i] == static_cast<float>(i + 1) );
	}
}

TEST_CASE_METHOD(
	cpu_copy_program_builder_fixture,
	"cpu_copy_program_builder builds a program that casts the source "
	"elements into the destination type",
	"[cpu_copy_program_builder]"
)
{
	const copy_operation operation;

	// The source is int32 while the destination is float32, so each element
	// must be converted while copying.
	const std::vector<array_signature> outputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};
	const std::vector<array_signature> inputs {
		make_contiguous_signature({4}, numerical_type::int32)
	};

	const auto program = build_program(operation, outputs, inputs);

	const auto destination =
		std::make_shared<hardware::host_buffer>(4 * sizeof(float), 64);
	const auto source =
		std::make_shared<hardware::host_buffer>(4 * sizeof(std::int32_t), 64);
	auto *dst_ptr = static_cast<float*>(destination->get_host_ptr());
	auto *src_ptr = static_cast<std::int32_t*>(source->get_host_ptr());
	for (std::size_t i = 0; i < 4; ++i)
	{
		dst_ptr[i] = 0.0f;
		src_ptr[i] = static_cast<std::int32_t>(i + 1);
	}

	execute(*program, destination, source);

	for (std::size_t i = 0; i < 4; ++i)
	{
		CHECK( dst_ptr[i] == static_cast<float>(i + 1) );
	}
}

TEST_CASE_METHOD(
	cpu_copy_program_builder_fixture,
	"cpu_copy_program_builder builds a program that honours a strided "
	"destination and leaves the gaps untouched",
	"[cpu_copy_program_builder]"
)
{
	const copy_operation operation;

	// Destination writes to elements 0, 2 and 4 of a 5-element buffer.
	const std::vector<array_signature> outputs {
		make_strided_signature({3}, {2}, numerical_type::float32)
	};
	const std::vector<array_signature> inputs {
		make_contiguous_signature({3}, numerical_type::float32)
	};

	const auto program = build_program(operation, outputs, inputs);

	const auto destination =
		std::make_shared<hardware::host_buffer>(5 * sizeof(float), 64);
	const auto source =
		std::make_shared<hardware::host_buffer>(3 * sizeof(float), 64);
	auto *dst_ptr = static_cast<float*>(destination->get_host_ptr());
	auto *src_ptr = static_cast<float*>(source->get_host_ptr());
	for (std::size_t i = 0; i < 5; ++i)
	{
		dst_ptr[i] = -1.0f;
	}
	for (std::size_t i = 0; i < 3; ++i)
	{
		src_ptr[i] = static_cast<float>(i + 1);
	}

	execute(*program, destination, source);

	CHECK( dst_ptr[0] == 1.0f );
	CHECK( dst_ptr[2] == 2.0f );
	CHECK( dst_ptr[4] == 3.0f );

	// The elements skipped by the stride keep their original value.
	CHECK( dst_ptr[1] == -1.0f );
	CHECK( dst_ptr[3] == -1.0f );
}

TEST_CASE_METHOD(
	cpu_copy_program_builder_fixture,
	"cpu_copy_program_builder::build throws when the source type can "
	"not be converted into the destination type",
	"[cpu_copy_program_builder]"
)
{
	const copy_operation operation;

	// A complex source can not be narrowed into a real destination.
	const std::vector<array_signature> outputs {
		make_contiguous_signature({4}, numerical_type::float32)
	};
	const std::vector<array_signature> inputs {
		make_contiguous_signature({4}, numerical_type::complex_float32)
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
