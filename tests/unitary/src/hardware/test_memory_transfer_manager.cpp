// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/hardware/memory_transfer_manager.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/hardware/memory_transfer.hpp>

#include "mock/mock_memory_transfer_backend.hpp"
#include "mock/mock_memory_transfer.hpp"
#include "mock/mock_memory_resource.hpp"

using namespace xmipp4;
using namespace xmipp4::hardware;

TEST_CASE( "memory_transfer_manager should allow registering backends", "[memory_transfer_backend]" )
{
    memory_transfer_manager manager;

    REQUIRE( manager.register_backend(std::move(std::make_unique<mock_memory_transfer_backend>())) == true );
    REQUIRE( manager.register_backend(std::move(std::make_unique<mock_memory_transfer_backend>())) == true );
}

TEST_CASE( "memory_transfer_manager does not allow registering null backend", "[memory_transfer_backend]" )
{
    memory_transfer_manager manager;

    REQUIRE( manager.register_backend( nullptr ) == false );
}

// TODO implement updated tests
