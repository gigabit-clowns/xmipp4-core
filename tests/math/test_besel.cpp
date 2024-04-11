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
 * @file test_bessel.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for bessel.hpp
 * @date 2024-04-11
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/math/bessel.hpp>

#include <map>

using namespace xmipp4::math;


TEST_CASE( "cylindrical_bessel_j0", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 1.0},
        {0.1, 0.997501562066040},
        {0.5, 0.938469807240813},
        {1.0, 0.765197686557967},
        {5.0, -0.177596771314338},
        {10.0, -0.245935764451348},
        {20.0, 0.167024664340583},
        {50.0, 0.055812327669252},
        {100.0, 0.019985850304223},

    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_j0(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_j0(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "cylindrical_bessel_j1", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.049937526036242},
        {0.5, 0.242268457674874},
        {1.0, 0.440050585744934},
        {5.0, -0.327579137591465},
        {10.0, 0.043472746168861},
        {20.0, 0.066833124175850},
        {50.0, -0.097511828125175},
        {100.0, -0.077145352014112},

    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_j1(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_j1(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}
