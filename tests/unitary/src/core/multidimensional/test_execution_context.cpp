// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/execution_context.hpp>

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>
#include <xmipp4/core/multidimensional/operation_command_manager.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>

#include <core/multidimensional/eager_operation_dispatcher.hpp>

#include "mock/mock_operation_dispatcher.hpp"
#include "../hardware/mock/mock_device.hpp"
#include "../hardware/mock/mock_memory_resource.hpp"
#include "../hardware/mock/mock_memory_allocator.hpp"
#include "../hardware/mock/mock_command_queue.hpp"

#include <memory>
#include <vector>
#include <stdexcept>

using namespace xmipp4::hardware;
using namespace xmipp4::multidimensional;

namespace
{

class execution_context_fixture
{
public:
	execution_context_fixture()
		: device(std::make_shared<mock_device>())
		, host_allocator(std::make_shared<mock_memory_allocator>())
		, device_allocator(std::make_shared<mock_memory_allocator>())
		, default_queue(std::make_shared<mock_command_queue>())
	{
	}

	std::shared_ptr<const device_context> build_device_context()
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

		return std::make_shared<device_context>(device);
	}

protected:
	std::shared_ptr<mock_device> device;
	std::shared_ptr<memory_allocator> host_allocator;
	std::shared_ptr<memory_allocator> device_allocator;
	mock_memory_resource host_resource;
	mock_memory_resource device_resource;
	std::shared_ptr<command_queue> default_queue;

private:
	std::vector<std::unique_ptr<trompeloeil::expectation>> m_expectations;
};

} // namespace



TEST_CASE(
	"execution_context default constructor leaves all attributes empty",
	"[execution_context]"
)
{
	const execution_context context;

	CHECK( context.get_device_context() == nullptr );
	CHECK( context.get_active_queue() == nullptr );
	CHECK( context.get_dispatcher() == nullptr );
}

TEST_CASE_METHOD(
	execution_context_fixture,
	"execution_context constructed with a dispatcher adopts the device's "
	"default queue",
	"[execution_context]"
)
{
	const auto device_ctx = build_device_context();
	const auto dispatcher = std::make_shared<mock_operation_dispatcher>();

	REQUIRE_CALL(*device, get_default_queue())
		.RETURN(default_queue);

	const execution_context context(device_ctx, dispatcher);

	CHECK( context.get_device_context() == device_ctx );
	CHECK( context.get_active_queue() == default_queue );
	CHECK( context.get_dispatcher() == dispatcher );
}

TEST_CASE_METHOD(
	execution_context_fixture,
	"execution_context constructed with a command manager creates an eager "
	"dispatcher",
	"[execution_context]"
)
{
	const auto device_ctx = build_device_context();
	const auto command_manager = std::make_shared<operation_command_manager>();

	REQUIRE_CALL(*device, get_default_queue())
		.RETURN(default_queue);

	const execution_context context(device_ctx, command_manager);

	CHECK( context.get_device_context() == device_ctx );
	CHECK( context.get_active_queue() == default_queue );
	REQUIRE( context.get_dispatcher() != nullptr );
	CHECK(
		dynamic_cast<const eager_operation_dispatcher*>(
			context.get_dispatcher().get()
		) != nullptr
	);
}

TEST_CASE(
	"execution_context constructor throws when the command manager is null",
	"[execution_context]"
)
{
	CHECK_THROWS_AS(
		execution_context(
			std::shared_ptr<const device_context>(),
			std::shared_ptr<const operation_command_manager>()
		),
		std::invalid_argument
	);
}

TEST_CASE_METHOD(
	execution_context_fixture,
	"execution_context on_device sets the device and adopts its default queue",
	"[execution_context]"
)
{
	const auto dispatcher = std::make_shared<mock_operation_dispatcher>();
	const execution_context base =
		execution_context().with_dispatcher(dispatcher);

	const auto device_ctx = build_device_context();

	REQUIRE_CALL(*device, get_default_queue())
		.RETURN(default_queue);

	const execution_context derived = base.on_device(device_ctx);

	CHECK( derived.get_device_context() == device_ctx );
	CHECK( derived.get_active_queue() == default_queue );
	CHECK( derived.get_dispatcher() == dispatcher );

	// The original context is left untouched.
	CHECK( base.get_device_context() == nullptr );
	CHECK( base.get_active_queue() == nullptr );
	CHECK( base.get_dispatcher() == dispatcher );
}

TEST_CASE_METHOD(
	execution_context_fixture,
	"execution_context on_device with a null device clears the device and "
	"queue",
	"[execution_context]"
)
{
	const auto device_ctx = build_device_context();
	const auto dispatcher = std::make_shared<mock_operation_dispatcher>();

	REQUIRE_CALL(*device, get_default_queue())
		.RETURN(default_queue);
	const execution_context base(device_ctx, dispatcher);

	const execution_context derived =
		base.on_device(std::shared_ptr<const device_context>());

	CHECK( derived.get_device_context() == nullptr );
	CHECK( derived.get_active_queue() == nullptr );
	CHECK( derived.get_dispatcher() == dispatcher );
}

TEST_CASE_METHOD(
	execution_context_fixture,
	"execution_context on_queue replaces the active queue and preserves the "
	"rest",
	"[execution_context]"
)
{
	const auto device_ctx = build_device_context();
	const auto dispatcher = std::make_shared<mock_operation_dispatcher>();

	REQUIRE_CALL(*device, get_default_queue())
		.RETURN(default_queue);
	const execution_context base(device_ctx, dispatcher);

	const std::shared_ptr<command_queue> other_queue =
		std::make_shared<mock_command_queue>();
	const execution_context derived = base.on_queue(other_queue);

	CHECK( derived.get_device_context() == device_ctx );
	CHECK( derived.get_active_queue() == other_queue );
	CHECK( derived.get_dispatcher() == dispatcher );

	// The original context still uses the default queue.
	CHECK( base.get_active_queue() == default_queue );
}

TEST_CASE_METHOD(
	execution_context_fixture,
	"execution_context with_dispatcher replaces the dispatcher and preserves "
	"the rest",
	"[execution_context]"
)
{
	const auto device_ctx = build_device_context();
	const auto dispatcher = std::make_shared<mock_operation_dispatcher>();

	REQUIRE_CALL(*device, get_default_queue())
		.RETURN(default_queue);
	const execution_context base(device_ctx, dispatcher);

	const auto other_dispatcher =
		std::make_shared<mock_operation_dispatcher>();
	const execution_context derived = base.with_dispatcher(other_dispatcher);

	CHECK( derived.get_device_context() == device_ctx );
	CHECK( derived.get_active_queue() == default_queue );
	CHECK( derived.get_dispatcher() == other_dispatcher );

	// The original context keeps its dispatcher.
	CHECK( base.get_dispatcher() == dispatcher );
}
