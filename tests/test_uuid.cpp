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
 * @file test_uuid.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for uuid.hpp
 * @date 2024-04-29
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/uuid.hpp>

#include <sstream>

using namespace xmipp4;


TEST_CASE( "output uuid to ostream", "[uuid]" ) 
{
    std::ostringstream oss;

    // Generate a pattern
    std::array<uint8_t, 16> data;
    for(std::size_t i = 0; i < data.size(); ++i)
    {
        data[i] = static_cast<uint8_t>((i<<4) | (15-i));
    }

    // Create the UUID and output it
    uuid id(data);
    oss << id;
    const auto uuid_as_str = oss.str();

    XMIPP4_CONST_CONSTEXPR const char expected[] = 
        "0F1E2D3C-4B5A-6978-8796-A5B4C3D2E1F0";
    REQUIRE( uuid_as_str == expected );
}
