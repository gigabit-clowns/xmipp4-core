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
 * @file test_byte_order.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for utils/byte_order.hpp
 * @date 2023-08-12
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/memory/byte_order.hpp>

using namespace xmipp4::memory;

TEST_CASE( "reverse byte order", "[byte_order]" ) 
{
    SECTION( "int8_t" )
    {
        REQUIRE( reverse_byte_order(int8_t(0x12)) == 0x12 );
    }
    SECTION( "uint8_t" )
    {
        REQUIRE( reverse_byte_order(uint8_t(0x12)) == 0x12 );
    }
    SECTION( "int16_t" )
    {
        REQUIRE( reverse_byte_order(int16_t(0x1234)) == 0x3412 );
    }
    SECTION( "uint16_t" )
    {
        REQUIRE( reverse_byte_order(uint16_t(0x1234)) == 0x3412 );
    }
    SECTION( "int32_t" )
    {
        REQUIRE( reverse_byte_order(int32_t(0x12345678)) == 0x78563412 );
    }
    SECTION( "uint32_t" )
    {
        REQUIRE( reverse_byte_order(uint32_t(0x12345678)) == 0x78563412 );
    }
    SECTION( "int64_t" )
    {
        REQUIRE( reverse_byte_order(int64_t(0x123456789ABCDEF0)) == 0xF0DEBC9A78563412 );
    }
    SECTION( "uint64_t" )
    {
        REQUIRE( reverse_byte_order(uint64_t(0x123456789ABCDEF0)) == 0xF0DEBC9A78563412 );
    }
}

