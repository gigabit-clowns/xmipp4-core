// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <communication/dummy/dummy_host_communicator_backend.hpp>

#include <xmipp4/core/core_version.hpp>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "dummy_host_communicator_backend should return dummy as its name", "[dummy_host_communicator_backend]" )
{
	dummy_host_communicator_backend backend;
	REQUIRE( backend.get_name() == "dummy" );
}

TEST_CASE( "dummy_host_communicator_backend should return the core version as its version", "[dummy_host_communicator_backend]" )
{
	dummy_host_communicator_backend backend;
	REQUIRE( backend.get_version() == get_core_version() );
}

TEST_CASE( "dummy_host_communicator_backend should return fallback as its suitability", "[dummy_host_communicator_backend]" )
{
	dummy_host_communicator_backend backend;
	REQUIRE( backend.get_suitability() == backend_priority::fallback );
}

TEST_CASE( "dummy_host_communicator_backend should return a valid world communicator", "[dummy_host_communicator_backend]" )
{
	dummy_host_communicator_backend backend;
	REQUIRE( backend.create_world_communicator() != nullptr );
}
