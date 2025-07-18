// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <xmipp4/core/multidimensional/broadcast.hpp>
#include <xmipp4/core/platform/compiler.h>

#include "mock/mock_broadcastable.hpp"

using namespace xmipp4::multidimensional;

TEST_CASE("broadcast should call broadcast_extents_to_layout and broadcast_layout_to_extents in order")
{
    mock_broadcastable first;
    mock_broadcastable second;
    mock_broadcastable third;

    std::vector<std::size_t> extents;
    
    #if defined(XMIPP4_GCC) || defined(XMIPP4_MSVC) // FIXME 
        REQUIRE_CALL(first, broadcast_extents_to_layout(std::ref(extents)));
        REQUIRE_CALL(second, broadcast_extents_to_layout(std::ref(extents)));
        REQUIRE_CALL(third, broadcast_extents_to_layout(std::ref(extents)));
        REQUIRE_CALL(first, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .RETURN(mock_broadcastable());
        REQUIRE_CALL(second, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .RETURN(mock_broadcastable());
        REQUIRE_CALL(third, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .RETURN(mock_broadcastable());
    #else
        trompeloeil::sequence seq;

        REQUIRE_CALL(first, broadcast_extents_to_layout(std::ref(extents)))
            .IN_SEQUENCE(seq);
        REQUIRE_CALL(second, broadcast_extents_to_layout(std::ref(extents)))
            .IN_SEQUENCE(seq);
        REQUIRE_CALL(third, broadcast_extents_to_layout(std::ref(extents)))
            .IN_SEQUENCE(seq);
        REQUIRE_CALL(first, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .RETURN(mock_broadcastable())
            .IN_SEQUENCE(seq);
        REQUIRE_CALL(second, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .RETURN(mock_broadcastable())
            .IN_SEQUENCE(seq);
        REQUIRE_CALL(third, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .RETURN(mock_broadcastable())
            .IN_SEQUENCE(seq);
    #endif

    broadcast(extents, first, second, third);
}
