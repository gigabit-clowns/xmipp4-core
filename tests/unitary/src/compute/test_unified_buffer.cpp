// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/unified_buffer.hpp>

#include "mock/mock_unified_buffer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

using namespace xmipp4;
using namespace xmipp4::compute;

TEST_CASE( "unified_buffer should return itself in get_host_accessible_alias", "[unified_buffer]" )
{
    mock_unified_buffer mock;
    const host_buffer *alias;
    
    alias = mock.get_host_accessible_alias();
    REQUIRE( alias == &mock );
    alias = static_cast<const mock_unified_buffer&>(mock).get_host_accessible_alias();
    REQUIRE( alias == &mock );
}

TEST_CASE( "unified_buffer should return itself in get_device_accessible_alias", "[unified_buffer]" )
{
    mock_unified_buffer mock;
    const device_buffer *alias;
    
    alias = mock.get_device_accessible_alias();
    REQUIRE( alias == &mock );
    alias = static_cast<const mock_unified_buffer&>(mock).get_device_accessible_alias();
    REQUIRE( alias == &mock );
}
