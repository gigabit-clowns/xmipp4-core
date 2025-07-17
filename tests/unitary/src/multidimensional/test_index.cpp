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
 * @file test_index.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for index.hpp
 * @date 2025-07-01
 * 
 */

#include <xmipp4/core/multidimensional/index.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>


using namespace xmipp4::multidimensional;



TEST_CASE("sanitize_index with a in-bounds positive index should return the same value", "[index]")
{
    const std::ptrdiff_t extent = 16;
    for (std::ptrdiff_t i = 0; i < extent; ++i)
    {
        const auto sanitized = sanitize_index(i, extent);
        REQUIRE( sanitized == i );
    }
}

TEST_CASE("sanitize_index with a in-bounds negative index should return the poisitivized value", "[index]")
{
    const std::ptrdiff_t extent = 16;
    for (std::ptrdiff_t i = 0; i < extent; ++i)
    {
        const auto index = -(i+1);
        const auto sanitized = sanitize_index(index, extent);
        REQUIRE( sanitized == (extent+index) ); // TODO parametrize
    }
}

TEST_CASE("sanitize_index with an out of bounds positive index should throw", "[index]")
{
    const std::ptrdiff_t extent = 16;
    const std::ptrdiff_t last = 32;
    for (std::ptrdiff_t i = extent; i < last; ++i)
    {
        std::ostringstream oss;
        oss << "Index " << i // TODO parametrize
            << " is out of bounds for extent " << extent;
        const auto err_msg = oss.str();

        REQUIRE_THROWS_AS( sanitize_index(i, extent), std::out_of_range );
        REQUIRE_THROWS_WITH( sanitize_index(i, extent), err_msg);
    }
}

TEST_CASE("sanitize_index with an out of bounds negative index should throw", "[index]")
{
    const std::ptrdiff_t extent = 16;
    const std::ptrdiff_t last = 32;
    for (std::ptrdiff_t i = extent; i < last; ++i)
    {
        const auto index = -(i+1);

        std::ostringstream oss; // TODO parametrize
        oss << "Reverse index " << index
            << " is out of bounds for extent " << extent;
        const auto err_msg = oss.str();

        REQUIRE_THROWS_AS( sanitize_index(index, extent), std::out_of_range );
        REQUIRE_THROWS_WITH( sanitize_index(index, extent), err_msg);
    }
}
