// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/hardware/device_instance.hpp>

#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>

#include "mock/mock_device.hpp"
#include "mock/mock_memory_resource.hpp"
#include "mock/mock_memory_allocator.hpp"
#include "mock/mock_command_queue.hpp"

#include <memory>
#include <vector>
#include <stdexcept>

using namespace xmipp4::hardware;

namespace
{

class device_instance_fixture
{
public:
	device_instance_fixture()
		: device(std::make_shared<mock_device>())
		, host_allocator(std::make_shared<mock_memory_allocator>())
		, device_allocator(std::make_shared<mock_memory_allocator>())
		, default_command_queue(std::make_shared<mock_command_queue>())
	{
	}

protected:
	std::shared_ptr<mock_device> device;
	std::shared_ptr<memory_allocator> host_allocator;
	std::shared_ptr<memory_allocator> device_allocator;
	std::shared_ptr<command_queue> default_command_queue;
	mock_memory_resource host_resource;
	mock_memory_resource device_resource;

	void expect_distinct_resources()
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
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(*device, create_command_queue())
			.RETURN(default_command_queue)
		);
	}

	void expect_shared_resource()
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
			.LR_RETURN(host_resource)
		);
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(host_resource, create_allocator())
			.RETURN(host_allocator)
		);
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(*device, create_command_queue())
			.RETURN(default_command_queue)
		);
	}

private:
	std::vector<std::unique_ptr<trompeloeil::expectation>> m_expectations;
};

} // namespace



TEST_CASE_METHOD(
	device_instance_fixture,
	"device_instance exposes the wrapped device and its properties",
	"[device_instance]"
)
{
	expect_distinct_resources();

	device_properties properties;
	properties.set_name("Test device");
	properties.set_type(device_type::gpu);

	const device_instance instance(device, properties);

	CHECK( instance.get_device() == device );
	CHECK( instance.get_properties().get_name() == "Test device" );
	CHECK( instance.get_properties().get_type() == device_type::gpu );
}

TEST_CASE_METHOD(
	device_instance_fixture,
	"device_instance selects an allocator for each affinity",
	"[device_instance]"
)
{
	expect_distinct_resources();

	const device_instance instance(device, device_properties{});

	CHECK(
		instance.get_allocator(memory_resource_affinity::host)
		== host_allocator
	);
	CHECK(
		instance.get_allocator(memory_resource_affinity::device)
		== device_allocator
	);
}

TEST_CASE_METHOD(
	device_instance_fixture,
	"device_instance shares one allocator when both affinities map to the same "
	"resource",
	"[device_instance]"
)
{
	expect_shared_resource();

	const device_instance instance(device, device_properties{});

	CHECK(
		instance.get_allocator(memory_resource_affinity::host)
		== host_allocator
	);
	CHECK(
		instance.get_allocator(memory_resource_affinity::device)
		== host_allocator
	);
	CHECK(
		instance.get_allocator(memory_resource_affinity::host)
		== instance.get_allocator(memory_resource_affinity::device)
	);
}

TEST_CASE_METHOD(
	device_instance_fixture,
	"device_instance shares creates a default command queue",
	"[device_instance]"
)
{
	expect_distinct_resources();
	const device_instance instance(device, device_properties{});

	CHECK( instance.get_default_queue() == default_command_queue );
}

TEST_CASE(
	"device_instance constructor throws when the device is null",
	"[device_instance]"
)
{
	CHECK_THROWS_AS(
		device_instance(std::shared_ptr<device>(), device_properties{}),
		std::invalid_argument
	);
}
