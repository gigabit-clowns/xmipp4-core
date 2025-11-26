// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/communication/device_communicator_manager.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "mock/mock_device_communicator_backend.hpp"
#include "mock/mock_device_communicator.hpp"
#include "mock/mock_host_communicator.hpp"
#include "../hardware/mock/mock_device.hpp"

#include <array>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "device_communicator_manager should allow registering backends", "[device_communicator_manager]" )
{
	device_communicator_manager manager;

	auto mock1 = std::make_unique<mock_device_communicator_backend>();
	REQUIRE_CALL(*mock1, get_name())
		.RETURN("mock1");
	auto mock2 = std::make_unique<mock_device_communicator_backend>();
	REQUIRE_CALL(*mock2, get_name())
		.RETURN("mock2");

	REQUIRE( manager.register_backend(std::move(mock1)) == true );
	REQUIRE( manager.register_backend(std::move(mock2)) == true );
}

TEST_CASE( "device_communicator_manager should not allow registering duplicate backends", "[device_communicator_manager]" )
{
	device_communicator_manager manager;

	auto mock = std::make_unique<mock_device_communicator_backend>();
	REQUIRE_CALL(*mock, get_name())
		.RETURN("mock");

	REQUIRE( manager.register_backend(std::move(mock)) == true );

	mock = std::make_unique<mock_device_communicator_backend>();
	REQUIRE_CALL(*mock, get_name())
		.RETURN("mock");
	REQUIRE( manager.register_backend(std::move(mock)) == false );
}

TEST_CASE( "device_communicator_manager should not allow registering null backend", "[device_communicator_manager]" )
{
	device_communicator_manager manager;

	REQUIRE( manager.register_backend( nullptr ) == false );
}

TEST_CASE( "default initialized device_communicator_manager should have no backends", "[device_communicator_manager]" )
{
	device_communicator_manager manager;

	auto backend1 = std::make_unique<mock_device_communicator_backend>();
	REQUIRE_CALL(*backend1, get_name())
		.RETURN("mock1");
	manager.register_backend(std::move(backend1));

	auto backend2 = std::make_unique<mock_device_communicator_backend>();
	REQUIRE_CALL(*backend2, get_name())
		.RETURN("mock2");
	manager.register_backend(std::move(backend2));

	std::vector<std::string> names;
	manager.enumerate_backends(names);
	std::sort(names.begin(), names.end());

	REQUIRE( names.size() == 2 );
	REQUIRE( names[0] == "mock1" );
	REQUIRE( names[1] == "mock2" );
}

TEST_CASE( "get_backend in device_communicator_manager should find an available backend", "[device_communicator_manager]" )
{
	device_communicator_manager manager;

	auto backend = std::make_unique<mock_device_communicator_backend>();
	auto backend_ptr = backend.get();
	REQUIRE_CALL(*backend, get_name())
		.RETURN("mock");
	manager.register_backend(std::move(backend));

	REQUIRE( manager.get_backend("mock") == backend_ptr );
}

TEST_CASE( "get_backend in device_communicator_manager should return null for inexistent backends", "[device_communicator_manager]" )
{
	device_communicator_manager manager;

	auto backend = std::make_unique<mock_device_communicator_backend>();
	REQUIRE_CALL(*backend, get_name())
		.RETURN("mock");
	manager.register_backend(std::move(backend));

	REQUIRE( manager.get_backend("not-a-mock") == nullptr );
}

TEST_CASE( "device_communicator_manager should enumerate its backends", "[device_communicator_manager]" )
{
	device_communicator_manager manager;
	std::vector<std::string> names;
	manager.enumerate_backends(names);
	REQUIRE( names.empty() );
}

TEST_CASE( "creating a device_communicator from a default initialized device_communicator_manager should throw", "[device_communicator_manager]" )
{
	device_communicator_manager manager;

	REQUIRE_THROWS_MATCHES(
		manager.create_world_communicators(nullptr, {}, {}),
		xmipp4::invalid_operation_error,
		Catch::Matchers::Message(
			"There is no available device_communicator_backend"
		)
	);
}

TEST_CASE( "device_communicator_manager should throw when there is no supported backend", "[device_communicator_manager]" )
{
	device_communicator_manager manager;

	auto backend1 = std::make_unique<mock_device_communicator_backend>();
	auto backend2 = std::make_unique<mock_device_communicator_backend>();

	hardware::mock_device device1;
	hardware::mock_device device2;
	mock_host_communicator node_communicator;
	std::array<hardware::device*, 2> devices = {
		&device1,
		&device2
	};

	REQUIRE_CALL(*backend1, get_name())
		.RETURN("mock1");
	REQUIRE_CALL(*backend1, get_suitability(ANY(xmipp4::span<hardware::device*>)))
		.LR_WITH(_1.data() == devices.data() && _1.size() == devices.size())
		.RETURN(xmipp4::backend_priority::unsupported);
	REQUIRE_CALL(*backend2, get_name())
		.RETURN("mock2");
	REQUIRE_CALL(*backend2, get_suitability(ANY(xmipp4::span<hardware::device*>)))
		.LR_WITH(_1.data() == devices.data() && _1.size() == devices.size())
		.RETURN(xmipp4::backend_priority::unsupported);

	manager.register_backend(std::move(backend1));
	manager.register_backend(std::move(backend2));

	std::array<std::shared_ptr<device_communicator>, 2> communicators;
	REQUIRE_THROWS_MATCHES(
		manager.create_world_communicators(
			&node_communicator, 
			xmipp4::make_span(devices), 
			xmipp4::make_span(communicators)
		),
		xmipp4::invalid_operation_error,
		Catch::Matchers::Message(
			"There is no available device_communicator_backend"
		)
	);
}

TEST_CASE( "device_communicator_manager should create the world communicator with the most suitable backend", "[device_communicator_manager]" )
{
	device_communicator_manager manager;

	auto backend1 = std::make_unique<mock_device_communicator_backend>();
	auto backend2 = std::make_unique<mock_device_communicator_backend>();

	hardware::mock_device device1;
	hardware::mock_device device2;
	mock_host_communicator node_communicator;
	std::array<hardware::device*, 2> devices = {
		&device1,
		&device2
	};
	auto communicator1 = std::make_shared<mock_device_communicator>();
	auto communicator2 = std::make_shared<mock_device_communicator>();
	std::array<std::shared_ptr<device_communicator>, 2> communicators;

	REQUIRE_CALL(*backend1, get_name())
		.RETURN("mock1");
	REQUIRE_CALL(*backend1, get_suitability(ANY(xmipp4::span<hardware::device*>)))
		.LR_WITH(_1.data() == devices.data() && _1.size() == devices.size())
		.RETURN(xmipp4::backend_priority::normal);
	REQUIRE_CALL(*backend2, get_name())
		.RETURN("mock2");
	REQUIRE_CALL(*backend2, get_suitability(ANY(xmipp4::span<hardware::device*>)))
		.LR_WITH(_1.data() == devices.data() && _1.size() == devices.size())
		.RETURN(xmipp4::backend_priority::optimal);
	REQUIRE_CALL(*backend2, create_world_communicators(&node_communicator, ANY(xmipp4::span<hardware::device*>), ANY(xmipp4::span<std::shared_ptr<device_communicator>>)))
		.LR_WITH(_2.data() == devices.data() && _2.size() == devices.size())
		.LR_WITH(_3.data() == communicators.data() && _3.size() == communicators.size())
		.LR_SIDE_EFFECT( _3[0] = communicator1 )
		.LR_SIDE_EFFECT( _3[1] = communicator2 );

	manager.register_backend(std::move(backend1));
	manager.register_backend(std::move(backend2));

	manager.create_world_communicators(
		&node_communicator, 
		xmipp4::make_span(devices),
		xmipp4::make_span(communicators)
	);
	REQUIRE( communicators[0] == communicator1 );
	REQUIRE( communicators[1] == communicator2 );
}

TEST_CASE( "creating a world communicator in device_communicator_manager with incorrect output size should throw", "[device_communicator_manager]" )
{
	device_communicator_manager manager;


	std::array<hardware::device*, 3> devices = {};
	std::array<std::shared_ptr<device_communicator>, 2> communicators = {};

	REQUIRE_THROWS_MATCHES(
		manager.create_world_communicators(
			nullptr, 
			xmipp4::make_span(devices),
			xmipp4::make_span(communicators)
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"Device array and output array must have matching sizes"
		)
	);
}
