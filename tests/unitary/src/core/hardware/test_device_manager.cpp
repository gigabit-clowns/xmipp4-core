// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/hardware/device_manager.hpp>

#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/device_instance.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/version.hpp>

#include <algorithm>
#include <memory>
#include <vector>

#include "mock/mock_device_backend.hpp"
#include "mock/mock_device.hpp"
#include "mock/mock_memory_resource.hpp"
#include "mock/mock_memory_allocator.hpp"

using namespace xmipp4;
using namespace xmipp4::hardware;

namespace
{

class device_manager_fixture
{
public:
	device_manager_fixture()
		: device(std::make_shared<mock_device>())
		, host_allocator(std::make_shared<mock_memory_allocator>())
		, device_allocator(std::make_shared<mock_memory_allocator>())
	{
	}

protected:
	device_manager manager;
	std::shared_ptr<mock_device> device;
	std::shared_ptr<memory_allocator> host_allocator;
	std::shared_ptr<memory_allocator> device_allocator;
	mock_memory_resource host_resource;
	mock_memory_resource device_resource;

	void register_named_backend(const std::string &name)
	{
		auto backend = std::make_unique<mock_device_backend>();
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(*backend, get_name())
			.RETURN(name)
		);
		manager.register_backend(std::move(backend));
	}

	void register_creating_backend(const std::string &name, std::size_t id)
	{
		auto backend = std::make_unique<mock_device_backend>();
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(*backend, get_name())
			.RETURN(name)
		);
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(*backend, create_device(id))
			.RETURN(device)
		);
		m_expectations.emplace_back(
			NAMED_REQUIRE_CALL(
				*backend,
				get_device_properties(id, trompeloeil::_)
			)
			.RETURN(true)
		);
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
		manager.register_backend(std::move(backend));
	}

private:
	std::vector<std::unique_ptr<trompeloeil::expectation>> m_expectations;
};

} // namespace



TEST_CASE(
	"device_manager should allow registering backends",
	"[device_manager]"
)
{
	device_manager manager;

	auto mock1 = std::make_unique<mock_device_backend>();
	REQUIRE_CALL(*mock1, get_name())
		.RETURN("mock1");
	auto mock2 = std::make_unique<mock_device_backend>();
	REQUIRE_CALL(*mock2, get_name())
		.RETURN("mock2");

	CHECK( manager.register_backend(std::move(mock1)) == true );
	CHECK( manager.register_backend(std::move(mock2)) == true );
}

TEST_CASE(
	"device_manager should not allow registering duplicate backends",
	"[device_manager]"
)
{
	device_manager manager;

	auto mock = std::make_unique<mock_device_backend>();
	REQUIRE_CALL(*mock, get_name())
		.RETURN("mock");

	CHECK( manager.register_backend(std::move(mock)) == true );

	mock = std::make_unique<mock_device_backend>();
	REQUIRE_CALL(*mock, get_name())
		.RETURN("mock");
	CHECK( manager.register_backend(std::move(mock)) == false );
}

TEST_CASE(
	"device_manager should not allow registering null backend",
	"[device_manager]"
)
{
	device_manager manager;

	CHECK( manager.register_backend(nullptr) == false );
}

TEST_CASE(
	"enumerate devices in device_manager should list all items",
	"[device_manager]"
)
{
	auto mock1 = std::make_unique<mock_device_backend>();
	const std::string name1 = "mock1";
	REQUIRE_CALL(*mock1, get_name())
		.RETURN(name1);
	REQUIRE_CALL(*mock1, enumerate_devices(ANY(std::vector<std::size_t>&)))
		.SIDE_EFFECT(_1 = {0, 1, 2, 3, 4});

	auto mock2 = std::make_unique<mock_device_backend>();
	const std::string name2 = "mock2";
	REQUIRE_CALL(*mock2, get_name())
		.RETURN(name2);
	REQUIRE_CALL(*mock2, enumerate_devices(ANY(std::vector<std::size_t>&)))
		.SIDE_EFFECT(_1 = {6, 7, 8, 9});

	device_manager manager;
	manager.register_backend(std::move(mock1));
	manager.register_backend(std::move(mock2));

	std::vector<device_index> devices;
	manager.enumerate_devices(devices);
	std::sort(devices.begin(), devices.end()); // Ordering not defined.

	REQUIRE( devices.size() == 9 );
	CHECK( devices[0] == device_index(name1, 0) );
	CHECK( devices[1] == device_index(name1, 1) );
	CHECK( devices[2] == device_index(name1, 2) );
	CHECK( devices[3] == device_index(name1, 3) );
	CHECK( devices[4] == device_index(name1, 4) );

	CHECK( devices[5] == device_index(name2, 6) );
	CHECK( devices[6] == device_index(name2, 7) );
	CHECK( devices[7] == device_index(name2, 8) );
	CHECK( devices[8] == device_index(name2, 9) );
}

TEST_CASE_METHOD(
	device_manager_fixture,
	"create_device_instance in device_manager routes to the matching backend "
	"and wraps its device in a device_instance",
	"[device_manager]"
)
{
	const std::size_t device_id = 154433421;
	register_named_backend("other");
	register_creating_backend("mock", device_id);

	const auto result = manager.create_device_instance(device_index("mock", device_id));

	REQUIRE( result != nullptr );
	CHECK( result->get_device() == device );
}

TEST_CASE_METHOD(
	device_manager_fixture,
	"create_device_instance in device_manager should throw when the backend "
	"does not exist",
	"[device_manager]"
)
{
	register_named_backend("mock");

	REQUIRE_THROWS_MATCHES(
		manager.create_device_instance(device_index("error", 0)),
		std::invalid_argument,
		Catch::Matchers::Message(
			"Requested backend does not exist"
		)
	);
}
