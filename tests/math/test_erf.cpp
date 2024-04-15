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
 * @file test_erf.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for erf.hpp
 * @date 2024-04-12
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/core/math/erf.hpp>

#include <map>
#include <set>

using namespace xmipp4::math;

TEST_CASE( "erf", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.112462916018285},
        {0.5, 0.520499877813047},
        {1.0, 0.842700792949715},
        {2.0, 0.995322265018953},
        {10.0, 1.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::erf(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::erf(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::erf(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::erf(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}

TEST_CASE( "erfc", "[math]" ) 
{
    const std::set<double> test_values = 
    {
        0.0, 0.1, 3.5, 1.5, 1.0, 100.0
    };

    for(const auto& sample : test_values)
    {
        REQUIRE( xmipp4::math::erfc(sample) == Catch::Approx(1.0 - xmipp4::math::erf(sample)) );
        REQUIRE( xmipp4::math::erfc(static_cast<float>(sample)) == Catch::Approx(1.0 - xmipp4::math::erf(sample)) );
        REQUIRE( xmipp4::math::erfc(-sample) == Catch::Approx(1.0 - xmipp4::math::erf(-sample)) );
        REQUIRE( xmipp4::math::erfc(-static_cast<float>(sample)) == Catch::Approx(1.0 - xmipp4::math::erf(-sample)) );
    }
}
