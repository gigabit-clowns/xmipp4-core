// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file test_device_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for device_buffer.hpp
 * @date 2024-11-09
 * 
 */

#include <xmipp4/core/compute/device_queue.hpp>

#include "mock/mock_device_buffer.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace xmipp4;
using namespace xmipp4::compute;

TEST_CASE( "get_host_accessible_alias should return null when null is provided", "[device_buffer]" )
{
    std::shared_ptr<device_buffer> buffer;
    REQUIRE( get_host_accessible_alias(buffer) == nullptr );
    std::shared_ptr<const device_buffer> const_buffer;
    REQUIRE( get_host_accessible_alias(const_buffer) == nullptr );
}

TEST_CASE( "get_host_accessible_alias should return null when buffer is not aliasable", "[device_buffer]" )
{
    auto mock = std::make_shared<mock_device_buffer>();

    REQUIRE_CALL(*mock, get_host_accessible_alias())
        .RETURN(nullptr)
        .TIMES(1);
    std::shared_ptr<device_buffer> buffer = mock;
    REQUIRE( get_host_accessible_alias(buffer) == nullptr );

    REQUIRE_CALL(static_cast<const mock_device_buffer&>(*mock), get_host_accessible_alias())
        .RETURN(nullptr)
        .TIMES(1);
    std::shared_ptr<const device_buffer> const_buffer = mock;
    REQUIRE( get_host_accessible_alias(const_buffer) == nullptr );
}

TEST_CASE( "get_host_accessible_alias should return the alias when buffer is aliasable", "[device_buffer]" )
{
    auto mock = std::make_shared<mock_device_buffer>();
    auto *alias = reinterpret_cast<host_buffer*>(std::uintptr_t(0xDEADBEEF)); 

    REQUIRE_CALL(*mock, get_host_accessible_alias())
        .RETURN(alias)
        .TIMES(1);
    std::shared_ptr<device_buffer> buffer = mock;
    REQUIRE( get_host_accessible_alias(buffer).get() == alias );

    REQUIRE_CALL(static_cast<const mock_device_buffer&>(*mock), get_host_accessible_alias())
        .RETURN(alias)
        .TIMES(1);
    std::shared_ptr<const device_buffer> const_buffer = mock;
    REQUIRE( get_host_accessible_alias(const_buffer).get() == alias );
}
