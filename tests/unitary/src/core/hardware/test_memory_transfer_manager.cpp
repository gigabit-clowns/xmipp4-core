// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/hardware/memory_transfer_manager.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/hardware/memory_transfer.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "mock/mock_memory_transfer_backend.hpp"
#include "mock/mock_memory_transfer.hpp"
#include "mock/mock_memory_resource.hpp"

using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE( "memory_transfer_manager should allow registering backends", "[memory_transfer_backend]" )
{
	memory_transfer_manager manager;

	REQUIRE( manager.register_backend(std::make_unique<mock_memory_transfer_backend>()) == true );
	REQUIRE( manager.register_backend(std::make_unique<mock_memory_transfer_backend>()) == true );
}

TEST_CASE( "memory_transfer_manager does not allow registering null backend", "[memory_transfer_backend]" )
{
	memory_transfer_manager manager;

	REQUIRE( manager.register_backend( nullptr ) == false );
}

TEST_CASE( "creating a transfer from a default initialized memory_transfer_manager should throw", "[memory_transfer_backend]" )
{
	memory_transfer_manager manager;

	mock_memory_resource source;
	mock_memory_resource destination;

	REQUIRE_THROWS_MATCHES(
		manager.create_transfer(source, destination),
		xmipp4::invalid_operation_error,
		Catch::Matchers::Message(
			"No backends were registered."
		)
	);
}

TEST_CASE( "memory_transfer_manager should use the most suitable backend", "[memory_transfer_backend]" )
{
	memory_transfer_manager manager;

	mock_memory_resource source;
	mock_memory_resource destination;
	auto transfer = std::make_shared<mock_memory_transfer>();

	auto backend1 = std::make_unique<mock_memory_transfer_backend>();
	auto *backend1_ref = backend1.get();
	auto backend2 = std::make_unique<mock_memory_transfer_backend>();
	auto *backend2_ref = backend2.get();
	manager.register_backend(std::move(backend1));
	manager.register_backend(std::move(backend2));

	trompeloeil::sequence seq;

	REQUIRE_CALL(*backend1_ref, get_suitability(ANY(const memory_resource&), ANY(const memory_resource&)))
		.LR_WITH(&_1 == &source && &_2 == &destination)
		.IN_SEQUENCE(seq)
		.RETURN(xmipp4::backend_priority::fallback);
	REQUIRE_CALL(*backend2_ref, get_suitability(ANY(const memory_resource&), ANY(const memory_resource&)))
		.LR_WITH(&_1 == &source && &_2 == &destination)
		.IN_SEQUENCE(seq)
		.RETURN(xmipp4::backend_priority::normal);
	REQUIRE_CALL(*backend2_ref, create_transfer(ANY(const memory_resource&), ANY(const memory_resource&)))
		.LR_WITH(&_1 == &source && &_2 == &destination)
		.IN_SEQUENCE(seq)
		.RETURN(transfer);

	REQUIRE( manager.create_transfer(source, destination) == transfer );
}

TEST_CASE( "memory_transfer_manager should cache the transfers", "[memory_transfer_backend]" )
{
	memory_transfer_manager manager;

	mock_memory_resource source;
	mock_memory_resource destination;
	auto transfer = std::make_shared<mock_memory_transfer>();

	auto backend1 = std::make_unique<mock_memory_transfer_backend>();
	auto *backend1_ref = backend1.get();
	auto backend2 = std::make_unique<mock_memory_transfer_backend>();
	auto *backend2_ref = backend2.get();
	manager.register_backend(std::move(backend1));
	manager.register_backend(std::move(backend2));
	
	{
		trompeloeil::sequence seq;

		REQUIRE_CALL(*backend1_ref, get_suitability(ANY(const memory_resource&), ANY(const memory_resource&)))
			.LR_WITH(&_1 == &source && &_2 == &destination)
			.RETURN(xmipp4::backend_priority::fallback);
		REQUIRE_CALL(*backend2_ref, get_suitability(ANY(const memory_resource&), ANY(const memory_resource&)))
			.LR_WITH(&_1 == &source && &_2 == &destination)
			.RETURN(xmipp4::backend_priority::normal);
		REQUIRE_CALL(*backend2_ref, create_transfer(ANY(const memory_resource&), ANY(const memory_resource&)))
			.LR_WITH(&_1 == &source && &_2 == &destination)
			.RETURN(transfer);
		
		manager.create_transfer(source, destination);
	}

	for (std::size_t i = 0; i < 12; ++i)
	{
		REQUIRE( manager.create_transfer(source, destination) == transfer );
	}
}

TEST_CASE( "memory_transfer_manager should throw when there is no supported backend", "[memory_transfer_backend]" )
{
	memory_transfer_manager manager;

	mock_memory_resource source;
	mock_memory_resource destination;
	auto transfer = std::make_shared<mock_memory_transfer>();

	auto backend1 = std::make_unique<mock_memory_transfer_backend>();
	auto backend2 = std::make_unique<mock_memory_transfer_backend>();

	REQUIRE_CALL(*backend1, get_suitability(ANY(const memory_resource&), ANY(const memory_resource&)))
		.LR_WITH(&_1 == &source && &_2 == &destination)
		.RETURN(xmipp4::backend_priority::unsupported);
	REQUIRE_CALL(*backend2, get_suitability(ANY(const memory_resource&), ANY(const memory_resource&)))
		.LR_WITH(&_1 == &source && &_2 == &destination)
		.RETURN(xmipp4::backend_priority::unsupported);

	manager.register_backend(std::move(backend1));
	manager.register_backend(std::move(backend2));

	REQUIRE_THROWS_MATCHES(
		manager.create_transfer(source, destination),
		xmipp4::invalid_operation_error,
		Catch::Matchers::Message(
			"No backend supports the requested transfer."
		)
	);
}
