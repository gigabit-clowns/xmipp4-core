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

TEST_CASE( "memory_transfer_manager should allow creating a supported transfer", "[memory_transfer_backend]" )
{
    memory_transfer_manager manager;
    
    mock_memory_resource src_resource;
    mock_memory_resource dst_resource;

    auto backend = std::make_unique<mock_memory_transfer_backend>();
    const auto expected_transfer = std::make_shared<mock_memory_transfer>();
    REQUIRE_CALL(*backend, create_transfer(ANY(const memory_resource&), ANY(const memory_resource&)))
        .LR_WITH(&_1 == &src_resource)
        .LR_WITH(&_2 == &dst_resource)
        .RETURN(expected_transfer);
    
    REQUIRE( manager.register_backend(std::move(backend)) == true );

    const auto transfer = manager.create_transfer(
        src_resource,
        dst_resource
    );
    REQUIRE( transfer == expected_transfer );
    
}   

TEST_CASE( "memory_transfer_manager should return nullptr when unsupported", "[memory_transfer_backend]" )
{
    memory_transfer_manager manager;
    
    mock_memory_resource src_resource;
    mock_memory_resource dst_resource;

    auto backend = std::make_unique<mock_memory_transfer_backend>();
    REQUIRE_CALL(*backend, create_transfer(ANY(const memory_resource&), ANY(const memory_resource&)))
        .LR_WITH(&_1 == &src_resource)
        .LR_WITH(&_2 == &dst_resource)
        .RETURN(nullptr);
    
    REQUIRE( manager.register_backend(std::move(backend)) == true );

    const auto transfer = manager.create_transfer(
        src_resource,
        dst_resource
    );
    REQUIRE( transfer == nullptr );
    
}   
