// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <hardware/host_memory/host_memory_allocator_backend.hpp>

#include <hardware/host_memory/host_memory_resource.hpp>

#include "../mock/mock_memory_resource.hpp"

#include <sstream>

using namespace xmipp4::hardware;

TEST_CASE( "host_memory_allocator_backend should have optimal priority for host_memory_resource", "[host_memory_allocator_backend]" )
{
	host_memory_allocator_backend backend;
	auto &host_memory = host_memory_resource::get();

	REQUIRE( backend.get_suitability(host_memory) == xmipp4::backend_priority::optimal );
}

TEST_CASE( "host_memory_allocator_backend should not support memory resources other than host_memory_resource", "[host_memory_allocator_backend]" )
{
	host_memory_allocator_backend backend;
	mock_memory_resource resource;

	REQUIRE( backend.get_suitability(resource) == xmipp4::backend_priority::unsupported );
}

TEST_CASE( "host_memory_allocator_backend should return a valid allocator when provided with correct parameters", "[host_memory_allocator_backend]" )
{
	host_memory_allocator_backend backend;
	auto &host_memory = host_memory_resource::get();

	auto allocator = backend.create_memory_allocator(host_memory);

	REQUIRE( allocator != nullptr );
}

TEST_CASE( "host_memory_allocator_backend should throw when trying to create an allocator with an unsupported memory_resource", "[host_memory_allocator_backend]" )
{
	host_memory_allocator_backend backend;
	mock_memory_resource resource;

	REQUIRE_THROWS_MATCHES(
		backend.create_memory_allocator(resource),
		std::invalid_argument,
		Catch::Matchers::Message(
			"Can not provide an allocator for a memory resource other than "
			"host_memory_resource"
		)
	);
}
