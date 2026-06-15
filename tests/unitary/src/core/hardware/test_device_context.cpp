// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/hardware/device_context.hpp>

#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>

#include "mock/mock_device.hpp"
#include "mock/mock_memory_resource.hpp"
#include "mock/mock_memory_allocator.hpp"
#include "mock/mock_command_queue.hpp"
#include "mock/mock_event.hpp"

#include <memory>
#include <vector>
#include <stdexcept>

using namespace xmipp4::hardware;

namespace
{

class device_context_fixture
{
public:
	device_context_fixture()
		: device(std::make_shared<mock_device>())
		, host_allocator(std::make_shared<mock_memory_allocator>())
		, device_allocator(std::make_shared<mock_memory_allocator>())
	{
	}

	device_context build_context()
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

		return device_context(device);
	}

protected:
	std::shared_ptr<mock_device> device;
	std::shared_ptr<memory_allocator> host_allocator;
	std::shared_ptr<memory_allocator> device_allocator;
	mock_memory_resource host_resource;
	mock_memory_resource device_resource;

private:
	std::vector<std::unique_ptr<trompeloeil::expectation>> m_expectations;
};

} // namespace



TEST_CASE(
	"device_context constructor should throw when given a null device",
	"[device_context]"
)
{
	CHECK_THROWS_AS(
		device_context(std::shared_ptr<device>()),
		std::invalid_argument
	);
}

TEST_CASE(
	"device_context default constructor should create an empty context",
	"[device_context]"
)
{
	device_context context;
	CHECK( context.is_empty() );
}

TEST_CASE_METHOD(
	device_context_fixture,
	"device_context constructor should build an allocator for each affinity",
	"[device_context]"
)
{
	const auto context = build_context();

	REQUIRE_FALSE( context.is_empty() );
	CHECK( context.get_device() == device );
	CHECK(
		context.get_allocator(memory_resource_affinity::host) == host_allocator
	);
	CHECK(
		context.get_allocator(memory_resource_affinity::device)
		== device_allocator
	);
}

TEST_CASE_METHOD(
	device_context_fixture,
	"device_context get_properties should forward to the wrapped device",
	"[device_context]"
)
{
	const auto context = build_context();

	CHECK( &context.get_properties() == &device->get_properties() );
}

TEST_CASE_METHOD(
	device_context_fixture,
	"device_context get_memory_resource should forward to the wrapped device",
	"[device_context]"
)
{
	const auto context = build_context();

	mock_memory_resource resource;
	REQUIRE_CALL(*device, get_memory_resource(memory_resource_affinity::device))
		.LR_RETURN(resource);

	CHECK(
		&context.get_memory_resource(memory_resource_affinity::device)
		== &resource
	);
}

TEST_CASE_METHOD(
	device_context_fixture,
	"device_context set_allocator should install and return the previous one",
	"[device_context]"
)
{
	auto context = build_context();

	std::shared_ptr<memory_allocator> replacement =
		std::make_shared<mock_memory_allocator>();
	const auto previous =
		context.set_allocator(memory_resource_affinity::host, replacement);

	CHECK( previous == host_allocator );
	CHECK(
		context.get_allocator(memory_resource_affinity::host) == replacement
	);
	CHECK(
		context.get_allocator(memory_resource_affinity::device)
		== device_allocator
	);
}

TEST_CASE_METHOD(
	device_context_fixture,
	"device_context set_allocator with nullptr should clear the slot",
	"[device_context]"
)
{
	auto context = build_context();

	const auto previous =
		context.set_allocator(memory_resource_affinity::host, nullptr);

	CHECK( previous == host_allocator );
	CHECK( context.get_allocator(memory_resource_affinity::host) == nullptr );
}

TEST_CASE_METHOD(
	device_context_fixture,
	"device_context get_default_queue should forward to the wrapped device",
	"[device_context]"
)
{
	const auto context = build_context();

	std::shared_ptr<command_queue> queue =
		std::make_shared<mock_command_queue>();
	REQUIRE_CALL(*device, get_default_queue())
		.RETURN(queue);

	CHECK( context.get_default_queue() == queue );
}

TEST_CASE_METHOD(
	device_context_fixture,
	"device_context create_command_queue should forward to the wrapped device",
	"[device_context]"
)
{
	const auto context = build_context();

	std::shared_ptr<command_queue> queue =
		std::make_shared<mock_command_queue>();
	REQUIRE_CALL(*device, create_command_queue())
		.RETURN(queue);

	CHECK( context.create_command_queue() == queue );
}

TEST_CASE_METHOD(
	device_context_fixture,
	"device_context create_event should forward the usage to the wrapped "
	"device",
	"[device_context]"
)
{
	const auto context = build_context();

	const event_usage_flags usage = {
		event_usage_flag_bits::host_wait,
		event_usage_flag_bits::device_wait
	};
	std::shared_ptr<event> created = std::make_shared<mock_event>();
	REQUIRE_CALL(*device, create_event(usage))
		.RETURN(created);

	CHECK( context.create_event(usage) == created );
}

TEST_CASE_METHOD(
	device_context_fixture,
	"device_context move constructor should transfer ownership and empty "
	"the source",
	"[device_context]"
)
{
	auto source = build_context();

	const device_context destination(std::move(source));

	CHECK( source.is_empty() );
	CHECK_FALSE( destination.is_empty() );
	CHECK( destination.get_device() == device );
	CHECK(
		destination.get_allocator(memory_resource_affinity::host)
		== host_allocator
	);
}

TEST_CASE_METHOD(
	device_context_fixture,
	"device_context move assignment should transfer ownership and empty "
	"the source",
	"[device_context]"
)
{
	auto source = build_context();

	device_context destination;
	REQUIRE( destination.is_empty() );

	destination = std::move(source);

	CHECK( source.is_empty() );
	CHECK_FALSE( destination.is_empty() );
	CHECK( destination.get_device() == device );
	CHECK(
		destination.get_allocator(memory_resource_affinity::device)
		== device_allocator
	);
}
