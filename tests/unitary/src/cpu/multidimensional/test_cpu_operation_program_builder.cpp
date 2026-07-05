// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <cpu/multidimensional/cpu_operation_program_builder.hpp>
#include <cpu/hardware/cpu_command_queue.hpp>

#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/operation_id.hpp>
#include <xmipp4/core/hardware/memory_resource_kind.hpp>
#include <xmipp4/core/backend_priority.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/span.hpp>

#include "../../core/multidimensional/mock/mock_operation.hpp"
#include "../../core/hardware/mock/mock_memory_resource.hpp"
#include "../../core/hardware/mock/mock_command_queue.hpp"

#include <memory>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

namespace
{

/**
 * @brief Concrete builder used to exercise get_suitability.
 *
 * cpu_operation_program_builder only overrides get_suitability and stays
 * abstract, so this subclass provides trivial stubs for the remaining pure
 * virtuals. Neither is invoked by the tests.
 */
class test_cpu_operation_program_builder final
	: public cpu_operation_program_builder
{
public:
	operation_id get_operation_id() const noexcept override
	{
		return operation_id::of<test_cpu_operation_program_builder>();
	}

	std::shared_ptr<hardware::program> build(
		const operation & /*operation*/,
		span<const array_signature> /*output_signatures*/,
		span<const array_signature> /*input_signatures*/,
		hardware::command_queue & /*queue*/,
		operation_program_cache * /*cache*/
	) const override
	{
		return nullptr;
	}
};

array_signature make_signature(const hardware::memory_resource *resource)
{
	return array_signature(
		strided_layout(),
		numerical_type::float32,
		resource
	);
}

} // namespace



TEST_CASE(
	"cpu_operation_program_builder get_suitability returns normal when every "
	"signature is host accessible and the queue is a CPU queue",
	"[cpu_operation_program_builder]"
)
{
	const test_cpu_operation_program_builder builder;
	const mock_operation operation;

	hardware::mock_memory_resource host_resource;
	ALLOW_CALL(host_resource, get_kind())
		.RETURN(hardware::memory_resource_kind::host);

	const std::vector<array_signature> outputs {
		make_signature(&host_resource)
	};
	const std::vector<array_signature> inputs {
		make_signature(&host_resource)
	};

	hardware::cpu_command_queue queue;

	const auto priority = builder.get_suitability(
		operation,
		make_span(outputs),
		make_span(inputs),
		queue
	);

	CHECK( priority == backend_priority::normal );
}

TEST_CASE(
	"cpu_operation_program_builder get_suitability returns unsupported when "
	"any output signature is not host accessible",
	"[cpu_operation_program_builder]"
)
{
	const test_cpu_operation_program_builder builder;
	const mock_operation operation;

	hardware::mock_memory_resource host_resource;
	ALLOW_CALL(host_resource, get_kind())
		.RETURN(hardware::memory_resource_kind::host);
	hardware::mock_memory_resource device_resource;
	ALLOW_CALL(device_resource, get_kind())
		.RETURN(hardware::memory_resource_kind::device_local);

	// The second output resides on device-local memory, so the whole set is
	// rejected even though the first one is host accessible.
	const std::vector<array_signature> outputs {
		make_signature(&host_resource),
		make_signature(&device_resource)
	};
	const std::vector<array_signature> inputs {
		make_signature(&host_resource)
	};

	hardware::cpu_command_queue queue;

	const auto priority = builder.get_suitability(
		operation,
		make_span(outputs),
		make_span(inputs),
		queue
	);

	CHECK( priority == backend_priority::unsupported );
}

TEST_CASE(
	"cpu_operation_program_builder get_suitability returns unsupported when "
	"any input signature is not host accessible",
	"[cpu_operation_program_builder]"
)
{
	const test_cpu_operation_program_builder builder;
	const mock_operation operation;

	hardware::mock_memory_resource host_resource;
	ALLOW_CALL(host_resource, get_kind())
		.RETURN(hardware::memory_resource_kind::host);
	hardware::mock_memory_resource device_resource;
	ALLOW_CALL(device_resource, get_kind())
		.RETURN(hardware::memory_resource_kind::device_local);

	const std::vector<array_signature> outputs {
		make_signature(&host_resource)
	};
	const std::vector<array_signature> inputs {
		make_signature(&device_resource)
	};

	hardware::cpu_command_queue queue;

	const auto priority = builder.get_suitability(
		operation,
		make_span(outputs),
		make_span(inputs),
		queue
	);

	CHECK( priority == backend_priority::unsupported );
}

TEST_CASE(
	"cpu_operation_program_builder get_suitability returns unsupported when a "
	"signature has no memory resource",
	"[cpu_operation_program_builder]"
)
{
	const test_cpu_operation_program_builder builder;
	const mock_operation operation;

	// A null memory resource cannot be host accessible.
	const std::vector<array_signature> outputs {
		make_signature(nullptr)
	};
	const std::vector<array_signature> inputs;

	hardware::cpu_command_queue queue;

	const auto priority = builder.get_suitability(
		operation,
		make_span(outputs),
		make_span(inputs),
		queue
	);

	CHECK( priority == backend_priority::unsupported );
}

TEST_CASE(
	"cpu_operation_program_builder get_suitability returns unsupported when "
	"the queue is not a CPU queue",
	"[cpu_operation_program_builder]"
)
{
	const test_cpu_operation_program_builder builder;
	const mock_operation operation;

	hardware::mock_memory_resource host_resource;
	ALLOW_CALL(host_resource, get_kind())
		.RETURN(hardware::memory_resource_kind::host);

	const std::vector<array_signature> outputs {
		make_signature(&host_resource)
	};
	const std::vector<array_signature> inputs {
		make_signature(&host_resource)
	};

	// A non-CPU queue fails the try_cast even with valid signatures.
	hardware::mock_command_queue queue;

	const auto priority = builder.get_suitability(
		operation,
		make_span(outputs),
		make_span(inputs),
		queue
	);

	CHECK( priority == backend_priority::unsupported );
}

TEST_CASE(
	"cpu_operation_program_builder get_suitability returns normal for empty "
	"signature lists on a CPU queue",
	"[cpu_operation_program_builder]"
)
{
	const test_cpu_operation_program_builder builder;
	const mock_operation operation;

	// With no signatures to reject, only the queue kind decides suitability.
	const std::vector<array_signature> outputs;
	const std::vector<array_signature> inputs;

	hardware::cpu_command_queue queue;

	const auto priority = builder.get_suitability(
		operation,
		make_span(outputs),
		make_span(inputs),
		queue
	);

	CHECK( priority == backend_priority::normal );
}
