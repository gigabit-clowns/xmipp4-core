// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <hardware/host_memory/host_memory_transfer_backend.hpp>

#include "../mock/mock_memory_resource.hpp"

using namespace xmipp4::hardware;



TEST_CASE( "host_memory_transfer_backend should have normal suitability when resources are host accessible", "[host_memory_transfer_backend]" )
{
    host_memory_transfer_backend backend;

    mock_memory_resource source;
    mock_memory_resource destination;

    REQUIRE_CALL(source, get_kind())
        .RETURN(memory_resource_kind::unified);
    REQUIRE_CALL(destination, get_kind())
        .RETURN(memory_resource_kind::host_staging);

    REQUIRE( backend.get_suitability(source, destination) == xmipp4::backend_priority::normal );

}

TEST_CASE( "host_memory_transfer_backend should be unsupported when the source resource is not host accessible", "[host_memory_transfer_backend]" )
{
    host_memory_transfer_backend backend;

    mock_memory_resource source;
    mock_memory_resource destination;

    REQUIRE_CALL(source, get_kind())
        .RETURN(memory_resource_kind::device_local);

    REQUIRE( backend.get_suitability(source, destination) == xmipp4::backend_priority::unsupported );

}

TEST_CASE( "host_memory_transfer_backend should be unsupported when the destination resource is not host accessible", "[host_memory_transfer_backend]" )
{
    host_memory_transfer_backend backend;

    mock_memory_resource source;
    mock_memory_resource destination;

    REQUIRE_CALL(source, get_kind())
        .RETURN(memory_resource_kind::host);
    REQUIRE_CALL(destination, get_kind())
        .RETURN(memory_resource_kind::device_local);

    REQUIRE( backend.get_suitability(source, destination) == xmipp4::backend_priority::unsupported );

}

TEST_CASE( "create_transfer in host_memory_transfer_backend should create a valid transfer when valid arguments are provided", "[host_memory_transfer_backend]" )
{
    host_memory_transfer_backend backend;

    mock_memory_resource resource;
    REQUIRE_CALL(resource, get_kind())
        .TIMES(2)
        .RETURN(memory_resource_kind::host);

    auto transfer = backend.create_transfer(resource, resource);
    REQUIRE( transfer );

}

TEST_CASE( "create_transfer in host_memory_transfer_backend should throw when a non host accessible source resource is provided", "[host_memory_transfer_backend]" )
{
    host_memory_transfer_backend backend;

    mock_memory_resource source;
    mock_memory_resource destination;
    REQUIRE_CALL(source, get_kind())
        .RETURN(memory_resource_kind::device_local);

    REQUIRE_THROWS_MATCHES(
        backend.create_transfer(source, destination),
		std::invalid_argument,
		Catch::Matchers::Message(
            "source memory resource is not host accessible"
        )
	);
}

TEST_CASE( "create_transfer in host_memory_transfer_backend should throw when a non host accessible destination resource is provided", "[host_memory_transfer_backend]" )
{
    host_memory_transfer_backend backend;

    mock_memory_resource source;
    mock_memory_resource destination;
    REQUIRE_CALL(source, get_kind())
        .RETURN(memory_resource_kind::host);
    REQUIRE_CALL(destination, get_kind())
        .RETURN(memory_resource_kind::device_local);

    REQUIRE_THROWS_MATCHES(
        backend.create_transfer(source, destination),
		std::invalid_argument,
		Catch::Matchers::Message(
            "destination memory resource is not host accessible"
        )
	);
}
