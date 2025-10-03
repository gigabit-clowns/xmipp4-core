// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/broadcast.hpp>

#include <xmipp4/core/multidimensional/broadcast_error.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

using namespace xmipp4::multidimensional;



TEST_CASE("broadcast_extents with compatible extents should succeed", "[broadcast]")
{
    std::vector<std::size_t> extents1 = {5, 1, 4, 3};
    std::vector<std::size_t> extents2 = {5, 6, 1, 3};
    broadcast_extents(extents1, extents2);

    const std::vector<std::size_t> expected = {5, 6, 4, 3};
    REQUIRE(extents1 == expected);
    REQUIRE(extents2 == expected);
}

TEST_CASE("broadcast_extents with missing trailing axis in the right should succeed", "[broadcast]")
{
    std::vector<std::size_t> extents1 = {5, 1, 4, 3};
    std::vector<std::size_t> extents2 = {6, 1, 3};
    broadcast_extents(extents1, extents2);

    const std::vector<std::size_t> expected = {5, 6, 4, 3};
    REQUIRE(extents1 == expected);
    REQUIRE(extents2 == expected);
}

TEST_CASE("broadcast_extents with missing trailing axis in the left should succeed", "[broadcast]")
{
    std::vector<std::size_t> extents1 = {1, 4, 3};
    std::vector<std::size_t> extents2 = {5, 6, 1, 3};
    broadcast_extents(extents1, extents2);

    const std::vector<std::size_t> expected = {5, 6, 4, 3};
    REQUIRE(extents1 == expected);
    REQUIRE(extents2 == expected);
}

TEST_CASE("broadcast_extents with incompatible axis should fail", "[broadcast]")
{
    std::vector<std::size_t> extents1 = {5, 1, 4, 3};
    std::vector<std::size_t> extents2 = {5, 6, 2, 3};

    REQUIRE_THROWS_AS( broadcast_extents(extents1, extents2), broadcast_error );
    REQUIRE_THROWS_WITH( broadcast_extents(extents1, extents2), "Broadcast error: extents (5, 6, 4, 3) and (5, 6, 2, 3) are not compatible." );
}
