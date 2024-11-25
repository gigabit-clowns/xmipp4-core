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
 * @file test_host_numerical_type.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for numerical type conversions.
 * @date 2024-11-19
 * 
 */

#include <xmipp4/core/compute/host/host_numerical_type.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>


using namespace xmipp4;
using namespace xmipp4::compute;

class invalid_numerical_type
{
};

TEST_CASE( "check host numerical types", "[host_numerical_type]" )
{
    REQUIRE( host_numerical_type<std::int8_t>::value() == numerical_type::int8 );
    REQUIRE( host_numerical_type<std::uint8_t>::value() == numerical_type::uint8 );
    REQUIRE( host_numerical_type<std::int16_t>::value() == numerical_type::int16 );
    REQUIRE( host_numerical_type<std::uint16_t>::value() == numerical_type::uint16 );
    REQUIRE( host_numerical_type<std::int32_t>::value() == numerical_type::int32 );
    REQUIRE( host_numerical_type<std::uint32_t>::value() == numerical_type::uint32 );
    REQUIRE( host_numerical_type<std::int64_t>::value() == numerical_type::int64 );
    REQUIRE( host_numerical_type<std::uint64_t>::value() == numerical_type::uint64 );
    REQUIRE( host_numerical_type<float16_t>::value() == numerical_type::float16 );
    REQUIRE( host_numerical_type<float32_t>::value() == numerical_type::float32 );
    REQUIRE( host_numerical_type<float64_t>::value() == numerical_type::float64 );
    REQUIRE( host_numerical_type<std::complex<float16_t>>::value() == numerical_type::complex_float16 );
    REQUIRE( host_numerical_type<std::complex<float32_t>>::value() == numerical_type::complex_float32 );
    REQUIRE( host_numerical_type<std::complex<float64_t>>::value() == numerical_type::complex_float64 );
    REQUIRE( host_numerical_type<invalid_numerical_type>::value() == numerical_type::unknown );
}
