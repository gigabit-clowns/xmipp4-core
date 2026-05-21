// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation_command_cache.hpp>

#include <memory>
#include <string>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE(
	"typed_operation_command_cache_key forwards to std::hash",
	"[operation_command_cache]"
)
{
	const typed_operation_command_cache_key<int> key(42);
	CHECK(key.hash() == std::hash<int>{}(42));
}

TEST_CASE(
	"typed_operation_command_cache_key exposes the wrapped value",
	"[operation_command_cache]"
)
{
	const typed_operation_command_cache_key<std::string> key("plan-1024");
	CHECK(key.get_key() == "plan-1024");
}

TEST_CASE(
	"typed_operation_command_cache_key equates same-typed keys by value",
	"[operation_command_cache]"
)
{
	const typed_operation_command_cache_key<int> a(7);
	const typed_operation_command_cache_key<int> b(7);
	const typed_operation_command_cache_key<int> c(8);

	CHECK(a.equals(b));
	CHECK_FALSE(a.equals(c));
}

TEST_CASE(
	"operation_command_cache reports the capacity it was built with",
	"[operation_command_cache]"
)
{
	const operation_command_cache cache(4);
	CHECK(cache.get_capacity() == 4);
}

TEST_CASE(
	"operation_command_cache returns nullptr when touching a missing key",
	"[operation_command_cache]"
)
{
	operation_command_cache cache(4);
	CHECK(cache.touch<int>(42) == nullptr);
}

TEST_CASE(
	"operation_command_cache returns the stored value on a hit",
	"[operation_command_cache]"
)
{
	operation_command_cache cache(4);
	const auto value = std::make_shared<int>(123);

	cache.store(42, value);

	CHECK(cache.touch<int>(42) == value);
}

TEST_CASE(
	"operation_command_cache replaces the value when the same key is stored "
	"twice",
	"[operation_command_cache]"
)
{
	operation_command_cache cache(4);
	const auto first = std::make_shared<int>(1);
	const auto second = std::make_shared<int>(2);

	cache.store(42, first);
	cache.store(42, second);

	CHECK(cache.touch<int>(42) == second);
}

TEST_CASE(
	"operation_command_cache keeps keys of different types in disjoint name "
	"spaces",
	"[operation_command_cache]"
)
{
	operation_command_cache cache(4);
	const auto value_int = std::make_shared<int>(1);
	const auto value_long = std::make_shared<int>(2);

	// Same numeric value, different key types.
	cache.store(int{42}, value_int);
	cache.store(long{42}, value_long);

	CHECK(cache.touch<int>(int{42}) == value_int);
	CHECK(cache.touch<int>(long{42}) == value_long);
}

TEST_CASE(
	"operation_command_cache evicts the oldest entry when storing past "
	"capacity",
	"[operation_command_cache]"
)
{
	operation_command_cache cache(2);
	const auto v1 = std::make_shared<int>(1);
	const auto v2 = std::make_shared<int>(2);
	const auto v3 = std::make_shared<int>(3);

	cache.store(1, v1);
	cache.store(2, v2);
	cache.store(3, v3);

	CHECK(cache.touch<int>(1) == nullptr);
	CHECK(cache.touch<int>(2) == v2);
	CHECK(cache.touch<int>(3) == v3);
}

TEST_CASE(
	"operation_command_cache promotes touched entries away from eviction",
	"[operation_command_cache]"
)
{
	operation_command_cache cache(2);
	const auto v1 = std::make_shared<int>(1);
	const auto v2 = std::make_shared<int>(2);
	const auto v3 = std::make_shared<int>(3);

	cache.store(1, v1);
	cache.store(2, v2);
	REQUIRE(cache.touch<int>(1) == v1); // Touch promotes key 1.
	cache.store(3, v3);                 // Should evict key 2, not key 1.

	CHECK(cache.touch<int>(1) == v1);
	CHECK(cache.touch<int>(2) == nullptr);
	CHECK(cache.touch<int>(3) == v3);
}

TEST_CASE(
	"operation_command_cache eviction crosses key-type partitions",
	"[operation_command_cache]"
)
{
	operation_command_cache cache(2);
	const auto v1 = std::make_shared<int>(1);
	const auto v2 = std::make_shared<int>(2);
	const auto v3 = std::make_shared<int>(3);

	cache.store(int{1}, v1);
	cache.store(long{2}, v2);
	cache.store(std::string("three"), v3);

	CHECK(cache.touch<int>(int{1}) == nullptr);
	CHECK(cache.touch<int>(long{2}) == v2);
	CHECK(cache.touch<int>(std::string("three")) == v3);
}
