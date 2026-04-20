// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/broadcast.hpp>

#include <xmipp4/core/multidimensional/broadcast_error.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

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

	REQUIRE_THROWS_MATCHES(
		broadcast_extents(extents1, extents2),
		broadcast_error,
		Catch::Matchers::Message("Broadcast error: extents (5, 6, 4, 3) and (5, 6, 2, 3) are not compatible.")
	);
}

TEST_CASE("is_broadcast_compatible with equal extents should return true", "[broadcast]")
{
	const std::vector<std::size_t> extents1 = {5, 6, 4, 3};
	const std::vector<std::size_t> extents2 = {5, 6, 4, 3};
	REQUIRE(is_broadcast_compatible(extents1, extents2));
}

TEST_CASE("is_broadcast_compatible with broadcast-compatible extents should return true", "[broadcast]")
{
	const std::vector<std::size_t> extents1 = {5, 1, 4, 3};
	const std::vector<std::size_t> extents2 = {5, 6, 1, 3};
	REQUIRE(is_broadcast_compatible(extents1, extents2));
}

TEST_CASE("is_broadcast_compatible with different rank extents should return true when compatible", "[broadcast]")
{
	const std::vector<std::size_t> extents1 = {5, 1, 4, 3};
	const std::vector<std::size_t> extents2 = {6, 1, 3};
	REQUIRE(is_broadcast_compatible(extents1, extents2));
}

TEST_CASE("is_broadcast_compatible with incompatible extents should return false", "[broadcast]")
{
	const std::vector<std::size_t> extents1 = {5, 1, 4, 3};
	const std::vector<std::size_t> extents2 = {5, 6, 2, 3};
	REQUIRE_FALSE(is_broadcast_compatible(extents1, extents2));
}

TEST_CASE("is_broadcast_compatible is symmetric", "[broadcast]")
{
	const std::vector<std::size_t> extents1 = {5, 1, 4, 3};
	const std::vector<std::size_t> extents2 = {5, 6, 1, 3};
	REQUIRE(is_broadcast_compatible(extents1, extents2) == is_broadcast_compatible(extents2, extents1));
}

TEST_CASE("is_broadcastable_to with equal extents should return true", "[broadcast]")
{
	const std::vector<std::size_t> from_extents = {5, 6, 4, 3};
	const std::vector<std::size_t> to_extents   = {5, 6, 4, 3};
	REQUIRE(is_broadcastable_to(from_extents, to_extents));
}

TEST_CASE("is_broadcastable_to with ones in source should return true", "[broadcast]")
{
	const std::vector<std::size_t> from_extents = {5, 1, 4, 1};
	const std::vector<std::size_t> to_extents   = {5, 6, 4, 3};
	REQUIRE(is_broadcastable_to(from_extents, to_extents));
}

TEST_CASE("is_broadcastable_to with smaller rank source should return true when compatible", "[broadcast]")
{
	const std::vector<std::size_t> from_extents = {1, 4, 3};
	const std::vector<std::size_t> to_extents   = {5, 6, 4, 3};
	REQUIRE(is_broadcastable_to(from_extents, to_extents));
}

TEST_CASE("is_broadcastable_to with larger rank source should return false", "[broadcast]")
{
	const std::vector<std::size_t> from_extents = {5, 6, 4, 3};
	const std::vector<std::size_t> to_extents   = {6, 4, 3};
	REQUIRE_FALSE(is_broadcastable_to(from_extents, to_extents));
}

TEST_CASE("is_broadcastable_to with mismatched non-unit extent should return false", "[broadcast]")
{
	const std::vector<std::size_t> from_extents = {5, 2, 4, 3};
	const std::vector<std::size_t> to_extents   = {5, 6, 4, 3};
	REQUIRE_FALSE(is_broadcastable_to(from_extents, to_extents));
}

TEST_CASE("is_broadcastable_to is not symmetric when source has non-unit extent matching target", "[broadcast]")
{
	const std::vector<std::size_t> from_extents = {1, 4, 3};
	const std::vector<std::size_t> to_extents   = {5, 4, 3};
	REQUIRE(is_broadcastable_to(from_extents, to_extents));
	REQUIRE_FALSE(is_broadcastable_to(to_extents, from_extents));
}
