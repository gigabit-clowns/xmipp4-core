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
 * @file test_flagset.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for utils/flagset.hpp
 * @date 2023-08-09
 * 
 */

#include <catch2/catch.hpp>

#include <limits>
#include <type_traits>
#include <cstdint>

#include <xmipp4/utils/flagset.hpp>
#include <xmipp4/utils/bit.hpp>

using namespace xmipp4;

// Definition of the flag bits and its flagset

enum class test_int32_flag_bits : int32_t
{
    a = utils::bit(0),
    b = utils::bit(1),
    c = utils::bit(2),

    // ...

    x = utils::bit(28),
    y = utils::bit(29),
    z = utils::bit(30),
};
using test_int32_flags = utils::flagset<test_int32_flag_bits>;

enum class test_uint16_flag_bits : uint16_t
{
    a = utils::bit(0),
    b = utils::bit(1),
    c = utils::bit(2),

    // ...

    x = utils::bit(13),
    y = utils::bit(14),
    z = utils::bit(15),
};
using test_uint16_flags = utils::flagset<test_uint16_flag_bits>;

enum class test_uint32_flag_bits : uint32_t
{
    a = utils::bit(0),
    b = utils::bit(1),
    c = utils::bit(2),

    // ...

    x = utils::bit(29),
    y = utils::bit(30),
    z = utils::bit(31),
};
using test_uint32_flags = utils::flagset<test_uint32_flag_bits>;



TEST_CASE( "flagset int32", "[flagset]" ) 
{
    test_int32_flags flags;

    SECTION("Empty constructor")
    {
        REQUIRE(flags == test_int32_flags());
        REQUIRE(static_cast<bool>(flags) == false);
    }

    SECTION("Bit constructor")
    {
        flags = test_int32_flag_bits::a; // Not explicit
        REQUIRE(flags.test(test_int32_flag_bits::a) == true);
        REQUIRE(flags.test(test_int32_flag_bits::b) == false);
    }

    SECTION("Raw constructor")
    {
        flags = test_int32_flags(0x50000005);
        REQUIRE(flags.test(test_int32_flag_bits::a) == true);
        REQUIRE(flags.test(test_int32_flag_bits::b) == false);
        REQUIRE(flags.test(test_int32_flag_bits::c) == true);
        REQUIRE(flags.test(test_int32_flag_bits::x) == true);
        REQUIRE(flags.test(test_int32_flag_bits::y) == false);
        REQUIRE(flags.test(test_int32_flag_bits::z) == true);
    }

    SECTION("Initializer list constructor")
    {
        flags = {
            test_int32_flag_bits::b,
            test_int32_flag_bits::y,
            test_int32_flag_bits::z,
        };
        REQUIRE(flags.test(test_int32_flag_bits::a) == false);
        REQUIRE(flags.test(test_int32_flag_bits::b) == true);
        REQUIRE(flags.test(test_int32_flag_bits::c) == false);
        REQUIRE(flags.test(test_int32_flag_bits::x) == false);
        REQUIRE(flags.test(test_int32_flag_bits::y) == true);
        REQUIRE(flags.test(test_int32_flag_bits::z) == true);
    }
    
    SECTION("Bool cast")
    {
        flags = {
            test_int32_flag_bits::b,
            test_int32_flag_bits::y,
            test_int32_flag_bits::z,
        };
        REQUIRE(static_cast<bool>(flags) == true);

        flags = {};
        REQUIRE(static_cast<bool>(flags) == false);
    }
    
    SECTION("Raw cast")
    {
        flags = {
            test_int32_flag_bits::b,
            test_int32_flag_bits::y,
            test_int32_flag_bits::z,
        };
        REQUIRE(static_cast<int32_t>(flags) == 0x60000002);
    }

    SECTION("All of")
    {
        flags = {
            test_int32_flag_bits::b,
            test_int32_flag_bits::y,
            test_int32_flag_bits::z,
        };
        REQUIRE(flags.all_of({test_int32_flag_bits::b, test_int32_flag_bits::z}) == true);
        REQUIRE(flags.all_of({test_int32_flag_bits::b, test_int32_flag_bits::x}) == false);
    }
    
    SECTION("Any of")
    {
        flags = {
            test_int32_flag_bits::b,
            test_int32_flag_bits::y,
            test_int32_flag_bits::z,
        };
        REQUIRE(flags.any_of({test_int32_flag_bits::b, test_int32_flag_bits::x}) == true);
        REQUIRE(flags.any_of({test_int32_flag_bits::c, test_int32_flag_bits::x}) == false);
    }
    
    SECTION("None of")
    {
        flags = {
            test_int32_flag_bits::b,
            test_int32_flag_bits::y,
            test_int32_flag_bits::z,
        };
        REQUIRE(flags.none_of({test_int32_flag_bits::a, test_int32_flag_bits::x}) == true);
        REQUIRE(flags.none_of({test_int32_flag_bits::a, test_int32_flag_bits::z}) == false);
    }
    
    SECTION("Only of")
    {
        flags = {
            test_int32_flag_bits::b,
            test_int32_flag_bits::y,
            test_int32_flag_bits::z,
        };
        REQUIRE(flags.only_of({test_int32_flag_bits::b, test_int32_flag_bits::y, test_int32_flag_bits::z}) == true);
        REQUIRE(flags.only_of({test_int32_flag_bits::b, test_int32_flag_bits::y, test_int32_flag_bits::z, test_int32_flag_bits::x }) == true);
        REQUIRE(flags.only_of({test_int32_flag_bits::a, test_int32_flag_bits::z}) == false);
    }

    SECTION("Test")
    {
        flags = {
            test_int32_flag_bits::b,
            test_int32_flag_bits::y,
            test_int32_flag_bits::z,
        };
        REQUIRE(flags.test(test_int32_flag_bits::b) == true);
        REQUIRE(flags.test(test_int32_flag_bits::c) == false);
    }
    
    SECTION("Count and parity")
    {
        flags = {
            test_int32_flag_bits::b,
            test_int32_flag_bits::y,
            test_int32_flag_bits::z,
        };
        REQUIRE(flags.count() == 3);
        REQUIRE(flags.parity() == true);

        flags.set(test_int32_flag_bits::x);
        REQUIRE(flags.count() == 4);
        REQUIRE(flags.parity() == false);
        
        flags.clear();
        REQUIRE(flags.count() == 0);
        REQUIRE(flags.parity() == false);
    }
    
    SECTION("Has single bit")
    {
        flags = {
            test_int32_flag_bits::b,
        };
        REQUIRE(flags.has_single_bit() == true);
        
        flags.set(test_int32_flag_bits::a);
        REQUIRE(flags.has_single_bit() == false);

        flags.clear();
        REQUIRE(flags.has_single_bit() == false);
    }

    SECTION("Set and clear")
    {
        flags = {
            test_int32_flag_bits::b,
        };
        flags.set({test_int32_flag_bits::a, test_int32_flag_bits::b, test_int32_flag_bits::c}, true);
        REQUIRE(flags == test_int32_flags({test_int32_flag_bits::a, test_int32_flag_bits::b, test_int32_flag_bits::c}));
        
        flags.set({test_int32_flag_bits::a, test_int32_flag_bits::x}, false);
        REQUIRE(flags == test_int32_flags({test_int32_flag_bits::b, test_int32_flag_bits::c}));
    }
    
    SECTION("Toggle")
    {
        flags = {
            test_int32_flag_bits::b,
        };
        flags.toggle({test_int32_flag_bits::a, test_int32_flag_bits::b, test_int32_flag_bits::c});
        REQUIRE(flags == test_int32_flags({test_int32_flag_bits::a, test_int32_flag_bits::c}));
    }
}