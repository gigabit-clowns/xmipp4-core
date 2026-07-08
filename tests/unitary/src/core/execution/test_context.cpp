// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/execution/context.hpp>

#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/device_instance.hpp>
#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>

#include "mock/mock_operation_dispatcher.hpp"
#include "../hardware/mock/mock_device.hpp"
#include "../hardware/mock/mock_memory_resource.hpp"
#include "../hardware/mock/mock_memory_allocator.hpp"
#include "../hardware/mock/mock_command_queue.hpp"

#include <memory>
#include <utility>
#include <stdexcept>

using namespace xmipp4;
using namespace xmipp4::execution;

namespace
{

class context_fixture
{
public:
	context_fixture()
		: device(std::make_shared<hardware::mock_device>())
		, host_allocator(std::make_shared<hardware::mock_memory_allocator>())
		, device_allocator(std::make_shared<hardware::mock_memory_allocator>())
		, default_queue(std::make_shared<hardware::mock_command_queue>())
		, dispatcher(std::make_shared<mock_operation_dispatcher>())
	{
		using hardware::memory_resource_affinity;

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

		instance = std::make_shared<hardware::device_instance>(
			device,
			hardware::device_properties()
		);
	}

protected:
	std::shared_ptr<hardware::mock_device> device;
	std::shared_ptr<hardware::memory_allocator> host_allocator;
	std::shared_ptr<hardware::memory_allocator> device_allocator;
	hardware::mock_memory_resource host_resource;
	hardware::mock_memory_resource device_resource;
	std::shared_ptr<hardware::command_queue> default_queue;
	std::shared_ptr<const hardware::device_instance> instance;
	std::shared_ptr<mock_operation_dispatcher> dispatcher;
};

} // namespace



TEST_CASE(
	"context default constructor produces an empty device context "
	"and a null dispatcher",
	"[context]"
)
{
	const context context;

	CHECK( context.get_device_context().get_device_instance() == nullptr );
	CHECK( context.get_dispatcher() == nullptr );
}

TEST_CASE_METHOD(
	context_fixture,
	"context constructor stores the provided device context and "
	"dispatcher",
	"[context]"
)
{
	const hardware::device_context dev_ctx(instance);

	const context context(dev_ctx, dispatcher);

	CHECK( context.get_device_context().get_device_instance() == instance );
	CHECK( context.get_dispatcher() == dispatcher );
}

TEST_CASE_METHOD(
	context_fixture,
	"context copy shares the device context and dispatcher and "
	"leaves the source intact",
	"[context]"
)
{
	const hardware::device_context dev_ctx(instance);
	const context original(dev_ctx, dispatcher);

	const context copy(original);

	CHECK( copy.get_device_context().get_device_instance() == instance );
	CHECK( copy.get_dispatcher() == dispatcher );

	// The source is left untouched.
	CHECK( original.get_device_context().get_device_instance() == instance );
	CHECK( original.get_dispatcher() == dispatcher );
}

TEST_CASE_METHOD(
	context_fixture,
	"context move transfers the device context and dispatcher",
	"[context]"
)
{
	const hardware::device_context dev_ctx(instance);
	context original(dev_ctx, dispatcher);

	const context moved(std::move(original));

	CHECK( moved.get_device_context().get_device_instance() == instance );
	CHECK( moved.get_dispatcher() == dispatcher );
}

TEST_CASE_METHOD(
	context_fixture,
	"context copy assignment replaces the target contents",
	"[context]"
)
{
	const hardware::device_context dev_ctx(instance);
	const context original(dev_ctx, dispatcher);

	context target;
	target = original;

	CHECK( target.get_device_context().get_device_instance() == instance );
	CHECK( target.get_dispatcher() == dispatcher );
}

TEST_CASE_METHOD(
	context_fixture,
	"context move assignment replaces the target contents",
	"[context]"
)
{
	const hardware::device_context dev_ctx(instance);
	context original(dev_ctx, dispatcher);

	context target;
	target = std::move(original);

	CHECK( target.get_device_context().get_device_instance() == instance );
	CHECK( target.get_dispatcher() == dispatcher );
}

TEST_CASE_METHOD(
	context_fixture,
	"context with_device_context replaces the device context, keeps "
	"the dispatcher and leaves the receiver unchanged",
	"[context]"
)
{
	const context base(hardware::device_context(), dispatcher);

	const auto derived =
		base.with_device_context(hardware::device_context(instance));

	CHECK( derived.get_device_context().get_device_instance() == instance );
	CHECK( derived.get_dispatcher() == dispatcher );

	// The receiver keeps its empty device context.
	CHECK( base.get_device_context().get_device_instance() == nullptr );
	CHECK( base.get_dispatcher() == dispatcher );
}

TEST_CASE(
	"context with_dispatcher swaps the dispatcher, preserves the "
	"device context and leaves the receiver unchanged",
	"[context]"
)
{
	const auto first = std::make_shared<mock_operation_dispatcher>();
	const auto second = std::make_shared<mock_operation_dispatcher>();
	const context base(hardware::device_context(), first);

	const auto derived = base.with_dispatcher(second);

	CHECK( derived.get_dispatcher() == second );
	// The (empty) device context is carried over unchanged.
	CHECK( derived.get_device_context().get_device_instance() == nullptr );

	// The receiver keeps its original dispatcher.
	CHECK( base.get_dispatcher() == first );
}
