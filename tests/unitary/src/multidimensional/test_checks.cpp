// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/checks.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <vector>
#include <numeric>
#include <random>
#include <algorithm>

using namespace xmipp4::multidimensional;



TEST_CASE("check_axis_permutation should not throw with a valid permutation", "[checks]")
{
    std::vector<std::size_t> permutation(16);
    std::iota(permutation.begin(), permutation.end(), 0UL);
    std::shuffle(permutation.begin(), permutation.end(), std::mt19937(0));

    REQUIRE_NOTHROW( check_axis_permutation(permutation.cbegin(), permutation.cend(), permutation.size()) );
}

TEST_CASE("check_axis_permutation throw with an incorrectly sized permutation", "[checks]")
{
    std::vector<std::size_t> permutation(16);
    std::iota(permutation.begin(), permutation.end(), 0UL);
    REQUIRE_THROWS_AS( check_axis_permutation(permutation.cbegin(), permutation.cend(), 10), std::invalid_argument );
    REQUIRE_THROWS_WITH( check_axis_permutation(permutation.cbegin(), permutation.cend(), 10), "permutation's length does not match the required count" );
}

TEST_CASE("check_axis_permutation throw with a missing index", "[checks]")
{
    std::vector<std::size_t> permutation(16);
    std::iota(permutation.begin(), permutation.end(), 0UL);
    permutation[15] = 16;

    REQUIRE_THROWS_AS( check_axis_permutation(permutation.cbegin(), permutation.cend(), permutation.size()), std::invalid_argument );
    REQUIRE_THROWS_WITH( check_axis_permutation(permutation.cbegin(), permutation.cend(), permutation.size()), "value 15 is missing in the permutation" );
}
