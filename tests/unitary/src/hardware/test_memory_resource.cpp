// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>

#include "mock/mock_device.hpp"
#include "mock/mock_memory_resource.hpp"

using namespace xmipp4::hardware;

TEST_CASE( "is_device_accessible should return true if the device's local memory resource is the provided memory resource", "[memory_resource]" )
{
	mock_device device;
	mock_memory_resource resource;

	REQUIRE_CALL( device, get_device_local_memory_resource() )
		.LR_RETURN(resource);

	REQUIRE( is_device_accessible(resource, device) == true );
}

TEST_CASE( "is_device_accessible should return true if the resource is device accessible and its target device is the same device", "[memory_resource]" )
{
	mock_device device;
	mock_memory_resource resource;
	mock_memory_resource another_resource;

	REQUIRE_CALL( device, get_device_local_memory_resource() )
		.LR_RETURN(another_resource);
	REQUIRE_CALL( resource, get_kind() )
		.RETURN( memory_resource_kind::device_local );
	REQUIRE_CALL( resource, get_target_device() )
		.LR_RETURN( &device );

	REQUIRE( is_device_accessible(resource, device) == true );
}

TEST_CASE( "is_device_accessible should return false if heuristics are not met", "[memory_resource]" )
{
	mock_device device;
	mock_memory_resource resource;
	mock_memory_resource another_resource;

	REQUIRE_CALL( device, get_device_local_memory_resource() )
		.LR_RETURN(another_resource);

	SECTION( "no target device" )
	{
		REQUIRE_CALL( resource, get_kind() )
			.RETURN( memory_resource_kind::device_local );
		REQUIRE_CALL( resource, get_target_device() )
			.RETURN( nullptr );
	}

	SECTION( "not device accessible" )
	{
		REQUIRE_CALL( resource, get_kind() )
			.RETURN( memory_resource_kind::host_staging );
		REQUIRE_CALL( resource, get_target_device() )
			.LR_RETURN( &device );
	}

	REQUIRE( is_device_accessible(resource, device) == false );
}
