// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/communication/host_communicator_manager.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "mock/mock_host_communicator_backend.hpp"

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "host_communicator_manager should allow registering backends", "[host_communicator_manager]" )
{
	host_communicator_manager manager;

	auto mock1 = std::make_unique<mock_host_communicator_backend>();
	REQUIRE_CALL(*mock1, get_name())
		.RETURN("mock1");
	auto mock2 = std::make_unique<mock_host_communicator_backend>();
	REQUIRE_CALL(*mock2, get_name())
		.RETURN("mock2");

	REQUIRE( manager.register_backend(std::move(mock1)) == true );
	REQUIRE( manager.register_backend(std::move(mock2)) == true );
}

TEST_CASE( "host_communicator_manager should not allow registering duplicate backends", "[host_communicator_manager]" )
{
	host_communicator_manager manager;

	auto mock = std::make_unique<mock_host_communicator_backend>();
	REQUIRE_CALL(*mock, get_name())
		.RETURN("mock");

	REQUIRE( manager.register_backend(std::move(mock)) == true );

	mock = std::make_unique<mock_host_communicator_backend>();
	REQUIRE_CALL(*mock, get_name())
		.RETURN("mock");
	REQUIRE( manager.register_backend(std::move(mock)) == false );
}

TEST_CASE( "host_communicator_manager should not allow registering null backend", "[host_communicator_manager]" )
{
	host_communicator_manager manager;

	REQUIRE( manager.register_backend( nullptr ) == false );
}

TEST_CASE( "creating a host_communicator from a default initialized host_communicator_manager should throw", "[host_communicator_manager]" )
{
	host_communicator_manager manager;

	REQUIRE_THROWS_MATCHES(
		manager.create_world_host_communicator(),
		xmipp4::invalid_operation_error,
		Catch::Matchers::Message(
			"No backends were registered."
		)
	);
}

TEST_CASE( "host_communicator_manager should throw when there is no supported backend", "[host_communicator_manager]" )
{
	host_communicator_manager manager;


	auto backend1 = std::make_unique<mock_host_communicator_backend>();
	auto backend2 = std::make_unique<mock_host_communicator_backend>();

	REQUIRE_CALL(*backend1, get_name())
		.RETURN("mock1");
	REQUIRE_CALL(*backend1, get_suitability())
		.RETURN(xmipp4::backend_priority::unsupported);
	REQUIRE_CALL(*backend2, get_name())
		.RETURN("mock2");
	REQUIRE_CALL(*backend2, get_suitability())
		.RETURN(xmipp4::backend_priority::unsupported);

	manager.register_backend(std::move(backend1));
	manager.register_backend(std::move(backend2));

	REQUIRE_THROWS_MATCHES(
		manager.create_world_host_communicator(),
		xmipp4::invalid_operation_error,
		Catch::Matchers::Message(
			"There is no available device host_communicator backend"
		)
	);
}
