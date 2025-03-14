/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file test_checks.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for checks.hpp
 * @date 2025-03-14
 * 
 */

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
