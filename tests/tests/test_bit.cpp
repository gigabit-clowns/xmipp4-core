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
 * @file test_bit.cpp
 * @author Oier Lauzirika Zarrabeitia
 * @brief Test for utils/bit.hpp
 * @date 2023-08-09
 */

#include <catch2/catch.hpp>

#include <limits>
#include <type_traits>
#include <cstdint>

#include <xmipp4/utils/bit.hpp>

using namespace xmipp4;

TEST_CASE( "bit definition", "[bit]" ) 
{
    REQUIRE( utils::bit(0) == 1UL );
    REQUIRE( utils::bit(1) == 2UL );
    REQUIRE( utils::bit(2) == 4UL );
    REQUIRE( utils::bit(8) == 256UL );
    XMIPP4_CONST_CONSTEXPR auto msb = std::numeric_limits<std::make_signed<std::size_t>::type>::min();
    REQUIRE( utils::bit(std::numeric_limits<std::size_t>::digits-1) == msb );
}

TEST_CASE( "set lowest zero", "[bit]" ) 
{
    SECTION( "8 bits" )
    {
        uint8_t i;

        SECTION( "set bits from LSB progressively" )
        {
            i = 0x00;
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x01 );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x03 );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x07 );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x0F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x1F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x3F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x7F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0xFF );
        }
        
        SECTION( "set bits from LSB progressively when there is a bit set elsewhere" )
        {
            i = 0x0A;
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x0B );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x0F );
        }

        SECTION( "edge case, when there are no zeros to be set" )
        {
            i = std::numeric_limits<uint8_t>::max();
            utils::set_lowest_zero_inplace(i); REQUIRE( i == std::numeric_limits<uint8_t>::max() );
        }
    }

    SECTION( "32 bits" )
    {
        uint32_t i;

        SECTION( "set bits from LSB progressively" )
        {
            i = 0x00;
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x01 );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x03 );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x07 );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x0F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x1F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x3F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x7F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0xFF );
        }
        
        SECTION( "set bits from LSB progressively when there is a bit set elsewhere" )
        {
            i = 0x0A;
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x0B );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x0F );
        }

        SECTION( "edge case, when there are no zeros to be set" )
        {
            i = std::numeric_limits<uint32_t>::max();
            utils::set_lowest_zero_inplace(i); REQUIRE( i == std::numeric_limits<uint32_t>::max() );
        }
    }

    SECTION( "64 bits" )
    {
        uint64_t i;

        SECTION( "set bits from LSB progressively" )
        {
            i = 0x00;
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x01 );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x03 );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x07 );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x0F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x1F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x3F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x7F );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0xFF );
        }
        
        SECTION( "set bits from LSB progressively when there is a bit set elsewhere" )
        {
            i = 0x0A;
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x0B );
            utils::set_lowest_zero_inplace(i); REQUIRE( i == 0x0F );
        }

        SECTION( "edge case, when there are no zeros to be set" )
        {
            i = std::numeric_limits<uint64_t>::max();
            utils::set_lowest_zero_inplace(i); REQUIRE( i == std::numeric_limits<uint64_t>::max() );
        }
    }
}

TEST_CASE( "clear lowest one", "[bit]" ) 
{
    SECTION( "8 bits" )
    {
        uint8_t i;

        SECTION( "clear bits from LSB progressively" )
        {
            i = 0xFF;
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xFE );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xFC );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xF8 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xF0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xE0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xC0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x80 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x00 );
        }
        
        SECTION( "clear bits from LSB progressively when there is a bit clear elsewhere" )
        {
            i = 0xDB;
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xDA );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xD8 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xD0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xC0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x80 );
        }

        SECTION( "edge case, when there are no ones to be cleared" )
        {
            i = 0x00;
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x00 );
        }
    }

    SECTION( "32 bits" )
    {
        uint32_t i;

        SECTION( "clear bits from LSB progressively" )
        {
            i = 0xFF;
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xFE );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xFC );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xF8 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xF0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xE0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xC0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x80 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x00 );
        }
        
        SECTION( "clear bits from LSB progressively when there is a bit clear elsewhere" )
        {
            i = 0xDB;
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xDA );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xD8 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xD0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xC0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x80 );
        }

        SECTION( "edge case, when there are no ones to be cleared" )
        {
            i = 0x00;
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x00 );
        }
    }

    SECTION( "64 bits" )
    {
        uint64_t i;

        SECTION( "clear bits from LSB progressively" )
        {
            i = 0xFF;
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xFE );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xFC );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xF8 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xF0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xE0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xC0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x80 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x00 );
        }
        
        SECTION( "clear bits from LSB progressively when there is a bit clear elsewhere" )
        {
            i = 0xDB;
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xDA );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xD8 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xD0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0xC0 );
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x80 );
        }

        SECTION( "edge case, when there are no ones to be cleared" )
        {
            i = 0x00;
            utils::clear_lowest_one_inplace(i); REQUIRE( i == 0x00 );
        }
    }
  
}

TEST_CASE( "mask trailing zeros", "[bit]" )
{
    SECTION( "8 bits" )
    {
        REQUIRE( utils::mask_trailing_zeros(uint8_t(0x01)) == uint8_t(0x00));
        REQUIRE( utils::mask_trailing_zeros(uint8_t(0x02)) == uint8_t(0x01));
        REQUIRE( utils::mask_trailing_zeros(uint8_t(0x04)) == uint8_t(0x03));
        REQUIRE( utils::mask_trailing_zeros(uint8_t(0xF4)) == uint8_t(0x03));
        REQUIRE( utils::mask_trailing_zeros(uint8_t(0xF0)) == uint8_t(0x0F));
        REQUIRE( utils::mask_trailing_zeros(uint8_t(0x00)) == uint8_t(0xFF));
    }
    SECTION( "32 bits" )
    {
        REQUIRE( utils::mask_trailing_zeros(uint32_t(0x00000001)) == uint32_t(0x00000000));
        REQUIRE( utils::mask_trailing_zeros(uint32_t(0x00000002)) == uint32_t(0x00000001));
        REQUIRE( utils::mask_trailing_zeros(uint32_t(0x00000004)) == uint32_t(0x00000003));
        REQUIRE( utils::mask_trailing_zeros(uint32_t(0xFFF40000)) == uint32_t(0x0003FFFF));
        REQUIRE( utils::mask_trailing_zeros(uint32_t(0xF0000000)) == uint32_t(0x0FFFFFFF));
        REQUIRE( utils::mask_trailing_zeros(uint32_t(0x00000000)) == uint32_t(0xFFFFFFFF));
    }
    SECTION( "64 bits" )
    {
        REQUIRE( utils::mask_trailing_zeros(uint64_t(0x0000000000000001)) == uint64_t(0x0000000000000000));
        REQUIRE( utils::mask_trailing_zeros(uint64_t(0x0000000000000002)) == uint64_t(0x0000000000000001));
        REQUIRE( utils::mask_trailing_zeros(uint64_t(0x0000000000000004)) == uint64_t(0x0000000000000003));
        REQUIRE( utils::mask_trailing_zeros(uint64_t(0xFFFFFFFFFFF40000)) == uint64_t(0x000000000003FFFF));
        REQUIRE( utils::mask_trailing_zeros(uint64_t(0xF000000000000000)) == uint64_t(0x0FFFFFFFFFFFFFFF));
        REQUIRE( utils::mask_trailing_zeros(uint64_t(0x0000000000000000)) == uint64_t(0xFFFFFFFFFFFFFFFF));
    }
}

TEST_CASE( "mask trailing ones", "[bit]" )
{
    SECTION( "8 bits" )
    {
        REQUIRE( utils::mask_trailing_ones(uint8_t(0x00)) == uint8_t(0x00));
        REQUIRE( utils::mask_trailing_ones(uint8_t(0x01)) == uint8_t(0x01));
        REQUIRE( utils::mask_trailing_ones(uint8_t(0x03)) == uint8_t(0x03));
        REQUIRE( utils::mask_trailing_ones(uint8_t(0xF3)) == uint8_t(0x03));
        REQUIRE( utils::mask_trailing_ones(uint8_t(0xFF)) == uint8_t(0xFF));
    }
    SECTION( "32 bits" )
    {
        REQUIRE( utils::mask_trailing_ones(uint32_t(0x00000000)) == uint32_t(0x00000000));
        REQUIRE( utils::mask_trailing_ones(uint32_t(0x00000001)) == uint32_t(0x00000001));
        REQUIRE( utils::mask_trailing_ones(uint32_t(0x00000003)) == uint32_t(0x00000003));
        REQUIRE( utils::mask_trailing_ones(uint32_t(0xFFFFFFF3)) == uint32_t(0x00000003));
        REQUIRE( utils::mask_trailing_ones(uint32_t(0xFFFFFFFF)) == uint32_t(0xFFFFFFFF));
    }
    SECTION( "64 bits" )
    {
        REQUIRE( utils::mask_trailing_ones(uint64_t(0x0000000000000000)) == uint64_t(0x0000000000000000));
        REQUIRE( utils::mask_trailing_ones(uint32_t(0x0000000000000001)) == uint32_t(0x0000000000000001));
        REQUIRE( utils::mask_trailing_ones(uint32_t(0x0000000000000003)) == uint32_t(0x0000000000000003));
        REQUIRE( utils::mask_trailing_ones(uint32_t(0xFFFFFFFFFFFFFFF3)) == uint32_t(0x0000000000000003));
        REQUIRE( utils::mask_trailing_ones(uint32_t(0xFFFFFFFFFFFFFFFF)) == uint32_t(0xFFFFFFFFFFFFFFFF));
    }
}

TEST_CASE( "rotate right", "[bit]" )
{
    // Online tool to rotate
    // https://onlinetoolz.net/bitshift
    SECTION( "8 bits" )
    {
        REQUIRE( utils::rotate_right(uint8_t(0xDE), 0)  == uint8_t(0xDE) );
        REQUIRE( utils::rotate_right(uint8_t(0xDE), 1)  == uint8_t(0x6F) );
        REQUIRE( utils::rotate_right(uint8_t(0xDE), -1) == uint8_t(0xBD) );
        REQUIRE( utils::rotate_right(uint8_t(0xDE), 2)  == uint8_t(0xB7) );
        REQUIRE( utils::rotate_right(uint8_t(0xDE), -2) == uint8_t(0x7B) );

        REQUIRE( utils::rotate_right(uint8_t(0xDE), 8)   == uint8_t(0xDE) );
        REQUIRE( utils::rotate_right(uint8_t(0xDE), 9)   == uint8_t(0x6F) );
        REQUIRE( utils::rotate_right(uint8_t(0xDE), -9)  == uint8_t(0xBD) );
        REQUIRE( utils::rotate_right(uint8_t(0xDE), 10)  == uint8_t(0xB7) );
        REQUIRE( utils::rotate_right(uint8_t(0xDE), -10) == uint8_t(0x7B) );
    }
    SECTION( "32 bits" )
    {
        REQUIRE( utils::rotate_right(uint32_t(0xDEADBEEF), 0)  == uint32_t(0xDEADBEEF) );
        REQUIRE( utils::rotate_right(uint32_t(0xDEADBEEF), 1)  == uint32_t(0xEF56DF77) );
        REQUIRE( utils::rotate_right(uint32_t(0xDEADBEEF), -1) == uint32_t(0xBD5B7DDF) );
        REQUIRE( utils::rotate_right(uint32_t(0xDEADBEEF), 2)  == uint32_t(0xF7AB6FBB) );
        REQUIRE( utils::rotate_right(uint32_t(0xDEADBEEF), -2) == uint32_t(0x7AB6FBBF) );

        REQUIRE( utils::rotate_right(uint32_t(0xDEADBEEF), 32)  == uint32_t(0xDEADBEEF) );
        REQUIRE( utils::rotate_right(uint32_t(0xDEADBEEF), 33)  == uint32_t(0xEF56DF77) );
        REQUIRE( utils::rotate_right(uint32_t(0xDEADBEEF), -33) == uint32_t(0xBD5B7DDF) );
        REQUIRE( utils::rotate_right(uint32_t(0xDEADBEEF), 34)  == uint32_t(0xF7AB6FBB) );
        REQUIRE( utils::rotate_right(uint32_t(0xDEADBEEF), -34) == uint32_t(0x7AB6FBBF) );
    }
    
    SECTION( "64 bits" )
    {
        REQUIRE( utils::rotate_right(uint64_t(0xDEADBEEF01234567), 0)  == uint64_t(0xDEADBEEF01234567) );
        REQUIRE( utils::rotate_right(uint64_t(0xDEADBEEF01234567), 1)  == uint64_t(0xEF56DF778091A2B3) );
        REQUIRE( utils::rotate_right(uint64_t(0xDEADBEEF01234567), -1) == uint64_t(0xBD5B7DDE02468ACF) );
        REQUIRE( utils::rotate_right(uint64_t(0xDEADBEEF01234567), 2)  == uint64_t(0xF7AB6FBBC048D159) );
        REQUIRE( utils::rotate_right(uint64_t(0xDEADBEEF01234567), -2) == uint64_t(0x7AB6FBBC048D159F) );

        REQUIRE( utils::rotate_right(uint64_t(0xDEADBEEF01234567), 64)  == uint64_t(0xDEADBEEF01234567) );
        REQUIRE( utils::rotate_right(uint64_t(0xDEADBEEF01234567), 65)  == uint64_t(0xEF56DF778091A2B3) );
        REQUIRE( utils::rotate_right(uint64_t(0xDEADBEEF01234567), -65) == uint64_t(0xBD5B7DDE02468ACF) );
        REQUIRE( utils::rotate_right(uint64_t(0xDEADBEEF01234567), 66)  == uint64_t(0xF7AB6FBBC048D159) );
        REQUIRE( utils::rotate_right(uint64_t(0xDEADBEEF01234567), -66) == uint64_t(0x7AB6FBBC048D159F) );
    }
}

TEST_CASE( "rotate left", "[bit]" )
{
    // Online tool to rotate
    // https://onlinetoolz.net/bitshift
    SECTION( "8 bits" )
    {
        REQUIRE( utils::rotate_left(uint8_t(0xDE), 0)  == uint8_t(0xDE) );
        REQUIRE( utils::rotate_left(uint8_t(0xDE), 1)  == uint8_t(0xBD) );
        REQUIRE( utils::rotate_left(uint8_t(0xDE), -1) == uint8_t(0x6F) );
        REQUIRE( utils::rotate_left(uint8_t(0xDE), 2)  == uint8_t(0x7B) );
        REQUIRE( utils::rotate_left(uint8_t(0xDE), -2) == uint8_t(0xB7) );

        REQUIRE( utils::rotate_left(uint8_t(0xDE), 8)   == uint8_t(0xDE) );
        REQUIRE( utils::rotate_left(uint8_t(0xDE), 9)   == uint8_t(0xBD) );
        REQUIRE( utils::rotate_left(uint8_t(0xDE), -9)  == uint8_t(0x6F) );
        REQUIRE( utils::rotate_left(uint8_t(0xDE), 10)  == uint8_t(0x7B) );
        REQUIRE( utils::rotate_left(uint8_t(0xDE), -10) == uint8_t(0xB7) );
    }
    SECTION( "32 bits" )
    {
        REQUIRE( utils::rotate_left(uint32_t(0xDEADBEEF), 0)  == uint32_t(0xDEADBEEF) );
        REQUIRE( utils::rotate_left(uint32_t(0xDEADBEEF), 1)  == uint32_t(0xBD5B7DDF) );
        REQUIRE( utils::rotate_left(uint32_t(0xDEADBEEF), -1) == uint32_t(0xEF56DF77) );
        REQUIRE( utils::rotate_left(uint32_t(0xDEADBEEF), 2)  == uint32_t(0x7AB6FBBF) );
        REQUIRE( utils::rotate_left(uint32_t(0xDEADBEEF), -2) == uint32_t(0xF7AB6FBB) );

        REQUIRE( utils::rotate_left(uint32_t(0xDEADBEEF), 32)   == uint32_t(0xDEADBEEF) );
        REQUIRE( utils::rotate_left(uint32_t(0xDEADBEEF), 33)   == uint32_t(0xBD5B7DDF) );
        REQUIRE( utils::rotate_left(uint32_t(0xDEADBEEF), -33)  == uint32_t(0xEF56DF77) );
        REQUIRE( utils::rotate_left(uint32_t(0xDEADBEEF), 34)   == uint32_t(0x7AB6FBBF) );
        REQUIRE( utils::rotate_left(uint32_t(0xDEADBEEF), -34)  == uint32_t(0xF7AB6FBB) );
    }
    
    SECTION( "64 bits" )
    {
        REQUIRE( utils::rotate_left(uint64_t(0xDEADBEEF01234567), 0)  == uint64_t(0xDEADBEEF01234567) );
        REQUIRE( utils::rotate_left(uint64_t(0xDEADBEEF01234567), -1) == uint64_t(0xEF56DF778091A2B3) );
        REQUIRE( utils::rotate_left(uint64_t(0xDEADBEEF01234567), 1)  == uint64_t(0xBD5B7DDE02468ACF) );
        REQUIRE( utils::rotate_left(uint64_t(0xDEADBEEF01234567), -2) == uint64_t(0xF7AB6FBBC048D159) );
        REQUIRE( utils::rotate_left(uint64_t(0xDEADBEEF01234567), 2)  == uint64_t(0x7AB6FBBC048D159F) );

        REQUIRE( utils::rotate_left(uint64_t(0xDEADBEEF01234567), 64)  == uint64_t(0xDEADBEEF01234567) );
        REQUIRE( utils::rotate_left(uint64_t(0xDEADBEEF01234567), 65)  == uint64_t(0xBD5B7DDE02468ACF) );
        REQUIRE( utils::rotate_left(uint64_t(0xDEADBEEF01234567), -65) == uint64_t(0xEF56DF778091A2B3) );
        REQUIRE( utils::rotate_left(uint64_t(0xDEADBEEF01234567), 66)  == uint64_t(0x7AB6FBBC048D159F) );
        REQUIRE( utils::rotate_left(uint64_t(0xDEADBEEF01234567), -66) == uint64_t(0xF7AB6FBBC048D159) );
    }
}

TEST_CASE( "has single bit", "[bit]" )
{
    REQUIRE( utils::has_single_bit(0U) == false );
    REQUIRE( utils::has_single_bit(1U) == true );
    REQUIRE( utils::has_single_bit(2U) == true );
    REQUIRE( utils::has_single_bit(3U) == false );
    REQUIRE( utils::has_single_bit(8U) == true );
    REQUIRE( utils::has_single_bit(32U) == true );
    REQUIRE( utils::has_single_bit(0x80000000) == true );
}

TEST_CASE( "bit floor", "[bit]" )
{
    REQUIRE( utils::bit_floor(0U) == 0U );
    REQUIRE( utils::bit_floor(1U) == 1U );
    REQUIRE( utils::bit_floor(2U) == 2U );
    REQUIRE( utils::bit_floor(3U) == 2U );
    REQUIRE( utils::bit_floor(4U) == 4U );
    REQUIRE( utils::bit_floor(5U) == 4U );
    REQUIRE( utils::bit_floor(31U) == 16U );
    REQUIRE( utils::bit_floor(0xFFFFFFFF) == 0x80000000 );
}

TEST_CASE( "bit ceil", "[bit]" )
{
    REQUIRE( utils::bit_ceil(0U) == 1U );
    REQUIRE( utils::bit_ceil(1U) == 1U );
    REQUIRE( utils::bit_ceil(2U) == 2U );
    REQUIRE( utils::bit_ceil(3U) == 4U );
    REQUIRE( utils::bit_ceil(4U) == 4U );
    REQUIRE( utils::bit_ceil(5U) == 8U );
    REQUIRE( utils::bit_ceil(31U) == 32U );
}

TEST_CASE( "bit width", "[bit]" )
{
    REQUIRE( utils::bit_width(0U) == 0U );
    REQUIRE( utils::bit_width(1U) == 1U );
    REQUIRE( utils::bit_width(2U) == 2U );
    REQUIRE( utils::bit_width(3U) == 2U );
    REQUIRE( utils::bit_width(4U) == 3U );
    REQUIRE( utils::bit_width(5U) == 3U );
    REQUIRE( utils::bit_width(31U) == 5U );
}

TEST_CASE( "count leading zeros", "[bit]" )
{
    SECTION( "8 bits" )
    {
        REQUIRE( utils::count_leading_zeros(uint8_t(0x00)) == 8);
        REQUIRE( utils::count_leading_zeros(uint8_t(0x01)) == 7);
        REQUIRE( utils::count_leading_zeros(uint8_t(0x02)) == 6);
        REQUIRE( utils::count_leading_zeros(uint8_t(0x08)) == 4);
        REQUIRE( utils::count_leading_zeros(uint8_t(0x03)) == 6);
        REQUIRE( utils::count_leading_zeros(uint8_t(0x55)) == 1);
        REQUIRE( utils::count_leading_zeros(uint8_t(0xAA)) == 0);
        REQUIRE( utils::count_leading_zeros(uint8_t(0xFF)) == 0);
    }
    SECTION( "32 bits" )
    {
        REQUIRE( utils::count_leading_zeros(uint32_t(0x00000000)) == 32);
        REQUIRE( utils::count_leading_zeros(uint32_t(0x00000001)) == 31);
        REQUIRE( utils::count_leading_zeros(uint32_t(0x00000002)) == 30);
        REQUIRE( utils::count_leading_zeros(uint32_t(0x00080000)) == 12);
        REQUIRE( utils::count_leading_zeros(uint32_t(0x03000000)) == 6);
        REQUIRE( utils::count_leading_zeros(uint32_t(0x55555555)) == 1);
        REQUIRE( utils::count_leading_zeros(uint32_t(0xAAAAAAAA)) == 0);
        REQUIRE( utils::count_leading_zeros(uint32_t(0xFFFFFFFF)) == 0);
    }
    SECTION( "64 bits" )
    {
        REQUIRE( utils::count_leading_zeros(uint64_t(0x0000000000000000)) == 64);
        REQUIRE( utils::count_leading_zeros(uint64_t(0x0000000000000001)) == 63);
        REQUIRE( utils::count_leading_zeros(uint64_t(0x0000000000000002)) == 62);
        REQUIRE( utils::count_leading_zeros(uint64_t(0x0008000000000000)) == 12);
        REQUIRE( utils::count_leading_zeros(uint64_t(0x0300300000010000)) == 6);
        REQUIRE( utils::count_leading_zeros(uint64_t(0x5555555555555555)) == 1);
        REQUIRE( utils::count_leading_zeros(uint64_t(0xAAAAAAAAAAAAAAAA)) == 0);
        REQUIRE( utils::count_leading_zeros(uint64_t(0xFFFFFFFFFFFFFFFF)) == 0);
    }
}

TEST_CASE( "count leading ones", "[bit]" )
{
    SECTION( "8 bits" )
    {
        REQUIRE( utils::count_leading_ones(uint8_t(0x00)) == 0);
        REQUIRE( utils::count_leading_ones(uint8_t(0x88)) == 1);
        REQUIRE( utils::count_leading_ones(uint8_t(0xC0)) == 2);
        REQUIRE( utils::count_leading_ones(uint8_t(0xE2)) == 3);
        REQUIRE( utils::count_leading_ones(uint8_t(0xF1)) == 4);
        REQUIRE( utils::count_leading_ones(uint8_t(0x55)) == 0);
        REQUIRE( utils::count_leading_ones(uint8_t(0xAA)) == 1);
        REQUIRE( utils::count_leading_ones(uint8_t(0xF0)) == 4);
        REQUIRE( utils::count_leading_ones(uint8_t(0xFF)) == 8);
    }
    SECTION( "32 bits" )
    {
        REQUIRE( utils::count_leading_ones(uint32_t(0x00000000)) == 0);
        REQUIRE( utils::count_leading_ones(uint32_t(0x88888888)) == 1);
        REQUIRE( utils::count_leading_ones(uint32_t(0xCD063678)) == 2);
        REQUIRE( utils::count_leading_ones(uint32_t(0xE2DFB245)) == 3);
        REQUIRE( utils::count_leading_ones(uint32_t(0xF157D22D)) == 4);
        REQUIRE( utils::count_leading_ones(uint32_t(0x55555555)) == 0);
        REQUIRE( utils::count_leading_ones(uint32_t(0xAAAAAAAA)) == 1);
        REQUIRE( utils::count_leading_ones(uint32_t(0xFFFFFFF0)) == 28);
        REQUIRE( utils::count_leading_ones(uint32_t(0xFFFFFFFF)) == 32);
    }
    SECTION( "64 bits" )
    {
        REQUIRE( utils::count_leading_ones(uint64_t(0x0000000000000000)) == 0);
        REQUIRE( utils::count_leading_ones(uint64_t(0x8888888888888888)) == 1);
        REQUIRE( utils::count_leading_ones(uint64_t(0xCD0612F3D43A6878)) == 2);
        REQUIRE( utils::count_leading_ones(uint64_t(0xE2DF124F6D4BB245)) == 3);
        REQUIRE( utils::count_leading_ones(uint64_t(0xF15782BF3D4DA22D)) == 4);
        REQUIRE( utils::count_leading_ones(uint64_t(0x5555555555555555)) == 0);
        REQUIRE( utils::count_leading_ones(uint64_t(0xAAAAAAAAAAAAAAAA)) == 1);
        REQUIRE( utils::count_leading_ones(uint64_t(0xFFFFFFFFFFFFFFF0)) == 60);
        REQUIRE( utils::count_leading_ones(uint64_t(0xFFFFFFFFFFFFFFFF)) == 64);
    }
}

TEST_CASE( "count trailing zeros", "[bit]" )
{
    SECTION( "8 bits" )
    {
        REQUIRE( utils::count_trailing_zeros(uint8_t(0x00)) == 8);
        REQUIRE( utils::count_trailing_zeros(uint8_t(0x01)) == 0);
        REQUIRE( utils::count_trailing_zeros(uint8_t(0x02)) == 1);
        REQUIRE( utils::count_trailing_zeros(uint8_t(0x08)) == 3);
        REQUIRE( utils::count_trailing_zeros(uint8_t(0x03)) == 0);
        REQUIRE( utils::count_trailing_zeros(uint8_t(0x55)) == 0);
        REQUIRE( utils::count_trailing_zeros(uint8_t(0xAA)) == 1);
        REQUIRE( utils::count_trailing_zeros(uint8_t(0x10)) == 4);
        REQUIRE( utils::count_trailing_zeros(uint8_t(0xFF)) == 0);
    }
    SECTION( "32 bits" )
    {
        REQUIRE( utils::count_trailing_zeros(uint32_t(0x00000000)) == 32);
        REQUIRE( utils::count_trailing_zeros(uint32_t(0x00000001)) == 0);
        REQUIRE( utils::count_trailing_zeros(uint32_t(0x00000002)) == 1);
        REQUIRE( utils::count_trailing_zeros(uint32_t(0x00080000)) == 19);
        REQUIRE( utils::count_trailing_zeros(uint32_t(0x03000000)) == 24);
        REQUIRE( utils::count_trailing_zeros(uint32_t(0x55555555)) == 0);
        REQUIRE( utils::count_trailing_zeros(uint32_t(0xAAAAAAAA)) == 1);
        REQUIRE( utils::count_trailing_zeros(uint32_t(0xFFFFFFFF)) == 0);
    }
    SECTION( "64 bits" )
    {
        REQUIRE( utils::count_trailing_zeros(uint64_t(0x0000000000000000)) == 64);
        REQUIRE( utils::count_trailing_zeros(uint64_t(0x0000000000000001)) == 0);
        REQUIRE( utils::count_trailing_zeros(uint64_t(0x0000000000000002)) == 1);
        REQUIRE( utils::count_trailing_zeros(uint64_t(0x0008000000000000)) == 51);
        REQUIRE( utils::count_trailing_zeros(uint64_t(0x0300300000010000)) == 16);
        REQUIRE( utils::count_trailing_zeros(uint64_t(0x5555555555555555)) == 0);
        REQUIRE( utils::count_trailing_zeros(uint64_t(0xAAAAAAAAAAAAAAAA)) == 1);
        REQUIRE( utils::count_trailing_zeros(uint64_t(0xFFFFFFFFFFFFFFFF)) == 0);
    }
}

TEST_CASE( "count trailing ones", "[bit]" )
{
    SECTION( "8 bits" )
    {
        REQUIRE( utils::count_trailing_ones(uint8_t(0x00)) == 0);
        REQUIRE( utils::count_trailing_ones(uint8_t(0x88)) == 0);
        REQUIRE( utils::count_trailing_ones(uint8_t(0x03)) == 2);
        REQUIRE( utils::count_trailing_ones(uint8_t(0x07)) == 3);
        REQUIRE( utils::count_trailing_ones(uint8_t(0x4F)) == 4);
        REQUIRE( utils::count_trailing_ones(uint8_t(0x55)) == 1);
        REQUIRE( utils::count_trailing_ones(uint8_t(0xAA)) == 0);
        REQUIRE( utils::count_trailing_ones(uint8_t(0x7F)) == 7);
        REQUIRE( utils::count_trailing_ones(uint8_t(0xFF)) == 8);
    }
    SECTION( "32 bits" )
    {
        REQUIRE( utils::count_trailing_ones(uint32_t(0x00000000)) == 0);
        REQUIRE( utils::count_trailing_ones(uint32_t(0x88888888)) == 0);
        REQUIRE( utils::count_trailing_ones(uint32_t(0x02ABC001)) == 1);
        REQUIRE( utils::count_trailing_ones(uint32_t(0x02ABC003)) == 2);
        REQUIRE( utils::count_trailing_ones(uint32_t(0x55555555)) == 1);
        REQUIRE( utils::count_trailing_ones(uint32_t(0xAAAAAAAA)) == 0);
        REQUIRE( utils::count_trailing_ones(uint32_t(0x0FFFFFFF)) == 28);
        REQUIRE( utils::count_trailing_ones(uint32_t(0xFFFFFFFF)) == 32);
    }
    SECTION( "64 bits" )
    {
        REQUIRE( utils::count_trailing_ones(uint64_t(0x0000000000000000)) == 0);
        REQUIRE( utils::count_trailing_ones(uint64_t(0x8888888888888888)) == 0);
        REQUIRE( utils::count_trailing_ones(uint64_t(0xCD0612F3D43A6873)) == 2);
        REQUIRE( utils::count_trailing_ones(uint64_t(0xE2DF124F6D4BB247)) == 3);
        REQUIRE( utils::count_trailing_ones(uint64_t(0xF15782BF3D4DA22F)) == 4);
        REQUIRE( utils::count_trailing_ones(uint64_t(0x5555555555555555)) == 1);
        REQUIRE( utils::count_trailing_ones(uint64_t(0xAAAAAAAAAAAAAAAA)) == 0);
        REQUIRE( utils::count_trailing_ones(uint64_t(0x0FFFFFFFFFFFFFFF)) == 60);
        REQUIRE( utils::count_trailing_ones(uint64_t(0xFFFFFFFFFFFFFFFF)) == 64);
    }
}

TEST_CASE( "popcount", "[bit]" )
{
    SECTION( "8 bits" )
    {
        REQUIRE( utils::popcount(uint8_t(0x00)) == 0);
        REQUIRE( utils::popcount(uint8_t(0x01)) == 1);
        REQUIRE( utils::popcount(uint8_t(0x02)) == 1);
        REQUIRE( utils::popcount(uint8_t(0x08)) == 1);
        REQUIRE( utils::popcount(uint8_t(0x03)) == 2);
        REQUIRE( utils::popcount(uint8_t(0x55)) == 4);
        REQUIRE( utils::popcount(uint8_t(0xAA)) == 4);
        REQUIRE( utils::popcount(uint8_t(0xFF)) == 8);
    }
    SECTION( "32 bits" )
    {
        REQUIRE( utils::popcount(uint32_t(0x00000000)) == 0);
        REQUIRE( utils::popcount(uint32_t(0x00000001)) == 1);
        REQUIRE( utils::popcount(uint32_t(0x00000002)) == 1);
        REQUIRE( utils::popcount(uint32_t(0x00080000)) == 1);
        REQUIRE( utils::popcount(uint32_t(0x03000000)) == 2);
        REQUIRE( utils::popcount(uint32_t(0x55555555)) == 16);
        REQUIRE( utils::popcount(uint32_t(0xAAAAAAAA)) == 16);
        REQUIRE( utils::popcount(uint32_t(0xFFFFFFFF)) == 32);
    }
    SECTION( "64 bits" )
    {
        REQUIRE( utils::popcount(uint64_t(0x0000000000000000)) == 0);
        REQUIRE( utils::popcount(uint64_t(0x0000000000000001)) == 1);
        REQUIRE( utils::popcount(uint64_t(0x0000000000000002)) == 1);
        REQUIRE( utils::popcount(uint64_t(0x0008000000000000)) == 1);
        REQUIRE( utils::popcount(uint64_t(0x0300300000010000)) == 5);
        REQUIRE( utils::popcount(uint64_t(0x5555555555555555)) == 32);
        REQUIRE( utils::popcount(uint64_t(0xAAAAAAAAAAAAAAAA)) == 32);
        REQUIRE( utils::popcount(uint64_t(0xFFFFFFFFFFFFFFFF)) == 64);
    }
}

TEST_CASE( "parity", "[bit]" )
{
    SECTION( "8 bits" )
    {
        REQUIRE( utils::parity(uint8_t(0x00)) == false);
        REQUIRE( utils::parity(uint8_t(0x01)) == true);
        REQUIRE( utils::parity(uint8_t(0x02)) == true);
        REQUIRE( utils::parity(uint8_t(0x08)) == true);
        REQUIRE( utils::parity(uint8_t(0x03)) == false);
        REQUIRE( utils::parity(uint8_t(0x55)) == false);
        REQUIRE( utils::parity(uint8_t(0xAA)) == false);
        REQUIRE( utils::parity(uint8_t(0xFF)) == false);
    }
    SECTION( "32 bits" )
    {
        REQUIRE( utils::parity(uint32_t(0x00000000)) == false);
        REQUIRE( utils::parity(uint32_t(0x00000001)) == true);
        REQUIRE( utils::parity(uint32_t(0x00000002)) == true);
        REQUIRE( utils::parity(uint32_t(0x00080000)) == true);
        REQUIRE( utils::parity(uint32_t(0x03040000)) == true);
        REQUIRE( utils::parity(uint32_t(0x55555555)) == false);
        REQUIRE( utils::parity(uint32_t(0xAAAAAAAA)) == false);
        REQUIRE( utils::parity(uint32_t(0xFFFFFFFF)) == false);
    }
    SECTION( "64 bits" )
    {
        REQUIRE( utils::parity(uint64_t(0x0000000000000000)) == false);
        REQUIRE( utils::parity(uint64_t(0x0000000000000001)) == true);
        REQUIRE( utils::parity(uint64_t(0x0000000000000002)) == true);
        REQUIRE( utils::parity(uint64_t(0x0008000000000000)) == true);
        REQUIRE( utils::parity(uint64_t(0x0300300000010000)) == true);
        REQUIRE( utils::parity(uint64_t(0x5555555555555555)) == false);
        REQUIRE( utils::parity(uint64_t(0xAAAAAAAAAAAAAAAA)) == false);
        REQUIRE( utils::parity(uint64_t(0xFFFFFFFFFFFFFFFF)) == false);
    }
}