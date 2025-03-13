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
 * @file test_axis_descriptor.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for multidimensional/axis_descriptor.hpp
 * @date 2024-05-08
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/axis_descriptor.hpp>

#include <array>

using namespace xmipp4::multidimensional;



TEST_CASE("make_contiguous_axis should create an axis of stride 1", "[axis_descriptor]")
{
    const auto axis = make_contiguous_axis(1234);
    REQUIRE( axis.get_extent() == 1234 );
    REQUIRE( axis.get_stride() == 1 );
}

TEST_CASE("make_phantom_axis should create an axis of stride 0", "[axis_descriptor]")
{
    const auto axis = make_phantom_axis(9876);
    REQUIRE( axis.get_extent() == 9876 );
    REQUIRE( axis.get_stride() == 0 );
}

TEST_CASE("check_nonzero_stride should only return true with non-zero strides", "[axis_descriptor]")
{
    REQUIRE( check_nonzero_stride(axis_descriptor(1234, 8)) );
    REQUIRE( check_nonzero_stride(axis_descriptor(1234, -8)) );
    REQUIRE( !check_nonzero_stride(axis_descriptor(1234, 0)) );
}

TEST_CASE("is_contiguous should return true only if the axis has a stride of 1", "[axis_descriptor]")
{
    REQUIRE( is_contiguous(axis_descriptor(1234, 1)) );
    REQUIRE( !is_contiguous(axis_descriptor(1234, 0)) );
    REQUIRE( !is_contiguous(axis_descriptor(1234, 8)) );
    REQUIRE( !is_contiguous(axis_descriptor(1234, -1)) );
}

TEST_CASE("is_contiguous should return true only if the product condition satisfies", "[axis_descriptor]")
{
    REQUIRE( is_contiguous(axis_descriptor(8, 2), axis_descriptor(1234, 16)) );
    REQUIRE( !is_contiguous(axis_descriptor(8, -2), axis_descriptor(1234, 16)) );
    REQUIRE( !is_contiguous(axis_descriptor(8, 2), axis_descriptor(1234, -16)) );
    REQUIRE( !is_contiguous(axis_descriptor(9, 2), axis_descriptor(1234, 16)) );
    REQUIRE( !is_contiguous(axis_descriptor(8, 2), axis_descriptor(1234, 31)) );
}

TEST_CASE("is_mirror_contiguous should return true only if the axis has a stride of 1 or -1", "[axis_descriptor]")
{
    REQUIRE( is_mirror_contiguous(axis_descriptor(1234, 1)) );
    REQUIRE( is_mirror_contiguous(axis_descriptor(1234, -1)) );
    REQUIRE( !is_mirror_contiguous(axis_descriptor(1234, 0)) );
    REQUIRE( !is_mirror_contiguous(axis_descriptor(1234, 8)) );
}

TEST_CASE("is_mirror_contiguous should return true only if the product condition satisfies", "[axis_descriptor]")
{
    REQUIRE( is_mirror_contiguous(axis_descriptor(8, 2), axis_descriptor(1234, 16)) );
    REQUIRE( is_mirror_contiguous(axis_descriptor(8, -2), axis_descriptor(1234, 16)) );
    REQUIRE( is_mirror_contiguous(axis_descriptor(8, 2), axis_descriptor(1234, -16)) );
    REQUIRE( is_mirror_contiguous(axis_descriptor(8, -2), axis_descriptor(1234, -16)) );
    REQUIRE( !is_mirror_contiguous(axis_descriptor(9, 2), axis_descriptor(1234, 16)) );
    REQUIRE( !is_mirror_contiguous(axis_descriptor(8, 2), axis_descriptor(1234, 31)) );
}

TEST_CASE("is_reversed should only return true when stride is negative", "[axis_descriptor]")
{
    REQUIRE( is_reversed(axis_descriptor(1234, -1)) );
    REQUIRE( is_reversed(axis_descriptor(1234, -45)) );
    REQUIRE( !is_reversed(axis_descriptor(1234, 0)) );
    REQUIRE( !is_reversed(axis_descriptor(1234, 83)) );
}

TEST_CASE("is_repeating should only return true for axes with an extent larger than 1 and an stride of zero", "[axis_descriptor]")
{
    REQUIRE( is_repeating(axis_descriptor(2, 0)) );
    REQUIRE( is_repeating(axis_descriptor(1743, 0)) );
    REQUIRE( !is_repeating(axis_descriptor(1, 0)) );
    REQUIRE( !is_repeating(axis_descriptor(0, 0)) );
    REQUIRE( !is_repeating(axis_descriptor(1244, 8)) );
    REQUIRE( !is_repeating(axis_descriptor(1244, -8)) );
}

TEST_CASE("is_empty should only return true for axes with an extent of zero", "[axis_descriptor]")
{
    REQUIRE( is_empty(axis_descriptor(0, 13)) );
    REQUIRE( is_empty(axis_descriptor(0, 0)) );
    REQUIRE( !is_empty(axis_descriptor(1, 0)) );
    REQUIRE( !is_empty(axis_descriptor(14, 0)) );
    REQUIRE( !is_empty(axis_descriptor(14, -18)) );
}

TEST_CASE("is_significant should only return true for axes with an extent unequal to 1", "[axis_descriptor]")
{
    REQUIRE( is_significant(axis_descriptor(12, 0)) );
    REQUIRE( is_significant(axis_descriptor(0, 0)) );
    REQUIRE( is_significant(axis_descriptor(12, 12)) );
    REQUIRE( is_significant(axis_descriptor(12, -12)) );
    REQUIRE( is_significant(axis_descriptor(43, 1)) );
    REQUIRE( is_significant(axis_descriptor(54, -1)) );
    REQUIRE( !is_significant(axis_descriptor(1, 0)) );
    REQUIRE( !is_significant(axis_descriptor(1, 18)) );
    REQUIRE( !is_significant(axis_descriptor(1, -18)) );
}

TEST_CASE("check_squeeze should only return true for axes with an extent equal to 1", "[axis_descriptor]")
{
    REQUIRE( check_squeeze(axis_descriptor(1, 0)) );
    REQUIRE( check_squeeze(axis_descriptor(1, 18)) );
    REQUIRE( check_squeeze(axis_descriptor(1, -18)) );
    REQUIRE( !check_squeeze(axis_descriptor(12, 0)) );
    REQUIRE( !check_squeeze(axis_descriptor(0, 0)) );
    REQUIRE( !check_squeeze(axis_descriptor(12, 12)) );
    REQUIRE( !check_squeeze(axis_descriptor(12, -12)) );
    REQUIRE( !check_squeeze(axis_descriptor(43, 1)) );
    REQUIRE( !check_squeeze(axis_descriptor(54, -1)) );
}

TEST_CASE("get_axis_last_offset should return the last offset for non-empty axes", "[axis_descriptor]")
{
    std::ptrdiff_t offset;
    REQUIRE( get_axis_last_offset(axis_descriptor(8, 10), offset) );
    REQUIRE( offset == 70 );
    REQUIRE( get_axis_last_offset(axis_descriptor(6, -10), offset) );
    REQUIRE( offset == -50 );
}

TEST_CASE("get_axis_last_offset should fail for empty axes", "[axis_descriptor]")
{
    std::ptrdiff_t offset = 1234567890;
    REQUIRE( !get_axis_last_offset(axis_descriptor(0, 10), offset) );
    REQUIRE( offset == 1234567890 );
}

TEST_CASE("get_axis_pivot_offset should return 0 for positive strided axes", "[axis_descriptor]")
{
    REQUIRE( get_axis_pivot_offset(axis_descriptor(1, 0)) == 0 );
    REQUIRE( get_axis_pivot_offset(axis_descriptor(18, 1)) == 0 );
    REQUIRE( get_axis_pivot_offset(axis_descriptor(18, 2384)) == 0 );
}

TEST_CASE("get_axis_pivot_offset should return 0 for empty axes", "[axis_descriptor]")
{
    REQUIRE( get_axis_pivot_offset(axis_descriptor(0, 0)) == 0 );
    REQUIRE( get_axis_pivot_offset(axis_descriptor(0, 1)) == 0 );
    REQUIRE( get_axis_pivot_offset(axis_descriptor(0, -1)) == 0 );
}

TEST_CASE("get_axis_pivot_offset should return the last offset for reversed axes", "[axis_descriptor]")
{
    REQUIRE( get_axis_pivot_offset(axis_descriptor(18, -1)) == 17 );
    REQUIRE( get_axis_pivot_offset(axis_descriptor(11, -10)) == 100 );
}

TEST_CASE("broadcast with no axes should always succeed", "[axis_descriptor]")
{
    REQUIRE( broadcast() );
}

TEST_CASE("broadcast with one axis should always succeed", "[axis_descriptor]")
{
    axis_descriptor axis0(3, 1);
    REQUIRE( broadcast(axis0) );
    REQUIRE( axis0 == axis_descriptor(3, 1) );
}

TEST_CASE("broadcast with two axes of equal extents should succeed", "[axis_descriptor]")
{
    axis_descriptor axis0(3, 1);
    axis_descriptor axis1(3, 2);
    REQUIRE( broadcast(axis0, axis1) );
    REQUIRE( axis0 == axis_descriptor(3, 1) );
    REQUIRE( axis1 == axis_descriptor(3, 2) );
}

TEST_CASE("broadcast with two compatible axes should succeed", "[axis_descriptor]")
{
    axis_descriptor axis0(3, 1);
    axis_descriptor axis1(1, 2);

    SECTION ("Ordered")
    {
        REQUIRE( broadcast(axis0, axis1) );
    }

    SECTION ("Reversed")
    {
        REQUIRE( broadcast(axis1, axis0) );
    }

    REQUIRE( axis0 == axis_descriptor(3, 1) );
    REQUIRE( axis1 == axis_descriptor(3, 0) );
}

TEST_CASE("broadcast with two incompatible axes should fail", "[axis_descriptor]")
{
    axis_descriptor axis0(3, 1);
    axis_descriptor axis1(2, 2);
    REQUIRE( !broadcast(axis1, axis0) );
}

TEST_CASE("broadcast with more than two compatible axes should succeed", "[axis_descriptor]")
{
    axis_descriptor axis0(1, 1);
    axis_descriptor axis1(3, 2);
    axis_descriptor axis2(1, 3);
    axis_descriptor axis3(3, 4);
    REQUIRE( broadcast(axis0, axis1, axis2, axis3) );
    REQUIRE( axis0 == axis_descriptor(3, 0) );
    REQUIRE( axis1 == axis_descriptor(3, 2) );
    REQUIRE( axis2 == axis_descriptor(3, 0) );
    REQUIRE( axis3 == axis_descriptor(3, 4) );
}

TEST_CASE("broadcast with more than two incompatible axes should fail", "[axis_descriptor]")
{
    axis_descriptor axis0(1, 1);
    axis_descriptor axis1(3, 2);
    axis_descriptor axis2(2, 3);
    axis_descriptor axis3(3, 4);
    REQUIRE( !broadcast(axis0, axis1, axis2, axis3) );
}
