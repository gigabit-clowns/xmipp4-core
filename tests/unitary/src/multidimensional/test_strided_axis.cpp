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
 * @file test_strided_axis.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for multidimensional/strided_axis.hpp
 * @date 2024-05-08
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/strided_axis.hpp>

#include <array>

using namespace xmipp4::multidimensional;



TEST_CASE("make_contiguous_axis should create an axis of stride 1", "[strided_axis]")
{
    const auto axis = make_contiguous_axis(1234);
    REQUIRE( axis.get_extent() == 1234 );
    REQUIRE( axis.get_stride() == 1 );
}

TEST_CASE("make_phantom_axis should create an axis of stride 0", "[strided_axis]")
{
    const auto axis = make_phantom_axis(9876);
    REQUIRE( axis.get_extent() == 9876 );
    REQUIRE( axis.get_stride() == 0 );
}

TEST_CASE("check_nonzero_stride should only return true with non-zero strides", "[strided_axis]")
{
    REQUIRE( check_nonzero_stride(strided_axis(1234, 8)) );
    REQUIRE( check_nonzero_stride(strided_axis(1234, -8)) );
    REQUIRE( !check_nonzero_stride(strided_axis(1234, 0)) );
}

TEST_CASE("is_contiguous should return true only if the axis has a stride of 1", "[strided_axis]")
{
    REQUIRE( is_contiguous(strided_axis(1234, 1)) );
    REQUIRE( !is_contiguous(strided_axis(1234, 0)) );
    REQUIRE( !is_contiguous(strided_axis(1234, 8)) );
    REQUIRE( !is_contiguous(strided_axis(1234, -1)) );
}

TEST_CASE("is_contiguous should return true only if the product condition satisfies", "[strided_axis]")
{
    REQUIRE( is_contiguous(strided_axis(8, 2), strided_axis(1234, 16)) );
    REQUIRE( !is_contiguous(strided_axis(8, -2), strided_axis(1234, 16)) );
    REQUIRE( !is_contiguous(strided_axis(8, 2), strided_axis(1234, -16)) );
    REQUIRE( !is_contiguous(strided_axis(9, 2), strided_axis(1234, 16)) );
    REQUIRE( !is_contiguous(strided_axis(8, 2), strided_axis(1234, 31)) );
}

TEST_CASE("is_reversed should only return true when stride is negative", "[strided_axis]")
{
    REQUIRE( is_reversed(strided_axis(1234, -1)) );
    REQUIRE( is_reversed(strided_axis(1234, -45)) );
    REQUIRE( !is_reversed(strided_axis(1234, 0)) );
    REQUIRE( !is_reversed(strided_axis(1234, 83)) );
}

TEST_CASE("is_repeating should only return true for axes with an extent larger than 1 and an stride of zero", "[strided_axis]")
{
    REQUIRE( is_repeating(strided_axis(2, 0)) );
    REQUIRE( is_repeating(strided_axis(1743, 0)) );
    REQUIRE( !is_repeating(strided_axis(1, 0)) );
    REQUIRE( !is_repeating(strided_axis(0, 0)) );
    REQUIRE( !is_repeating(strided_axis(1244, 8)) );
    REQUIRE( !is_repeating(strided_axis(1244, -8)) );
}

TEST_CASE("is_empty should only return true for axes with an extent of zero", "[strided_axis]")
{
    REQUIRE( is_empty(strided_axis(0, 13)) );
    REQUIRE( is_empty(strided_axis(0, 0)) );
    REQUIRE( !is_empty(strided_axis(1, 0)) );
    REQUIRE( !is_empty(strided_axis(14, 0)) );
    REQUIRE( !is_empty(strided_axis(14, -18)) );
}

TEST_CASE("is_significant should only return true for axes with an extent unequal to 1", "[strided_axis]")
{
    REQUIRE( is_significant(strided_axis(12, 0)) );
    REQUIRE( is_significant(strided_axis(0, 0)) );
    REQUIRE( is_significant(strided_axis(12, 12)) );
    REQUIRE( is_significant(strided_axis(12, -12)) );
    REQUIRE( is_significant(strided_axis(43, 1)) );
    REQUIRE( is_significant(strided_axis(54, -1)) );
    REQUIRE( !is_significant(strided_axis(1, 0)) );
    REQUIRE( !is_significant(strided_axis(1, 18)) );
    REQUIRE( !is_significant(strided_axis(1, -18)) );
}

TEST_CASE("check_squeeze should only return true for axes with an extent equal to 1", "[strided_axis]")
{
    REQUIRE( check_squeeze(strided_axis(1, 0)) );
    REQUIRE( check_squeeze(strided_axis(1, 18)) );
    REQUIRE( check_squeeze(strided_axis(1, -18)) );
    REQUIRE( !check_squeeze(strided_axis(12, 0)) );
    REQUIRE( !check_squeeze(strided_axis(0, 0)) );
    REQUIRE( !check_squeeze(strided_axis(12, 12)) );
    REQUIRE( !check_squeeze(strided_axis(12, -12)) );
    REQUIRE( !check_squeeze(strided_axis(43, 1)) );
    REQUIRE( !check_squeeze(strided_axis(54, -1)) );
}

TEST_CASE("get_axis_last_offset should return the last offset for non-empty axes", "[strided_axis]")
{
    std::ptrdiff_t offset;
    REQUIRE( get_axis_last_offset(strided_axis(8, 10), offset) );
    REQUIRE( offset == 70 );
    REQUIRE( get_axis_last_offset(strided_axis(6, -10), offset) );
    REQUIRE( offset == -50 );
}

TEST_CASE("get_axis_last_offset should fail for empty axes", "[strided_axis]")
{
    std::ptrdiff_t offset = 1234567890;
    REQUIRE( !get_axis_last_offset(strided_axis(0, 10), offset) );
    REQUIRE( offset == 1234567890 );
}

TEST_CASE("get_axis_pivot_offset should return 0 for positive strided axes", "[strided_axis]")
{
    REQUIRE( get_axis_pivot_offset(strided_axis(1, 0)) == 0 );
    REQUIRE( get_axis_pivot_offset(strided_axis(18, 1)) == 0 );
    REQUIRE( get_axis_pivot_offset(strided_axis(18, 2384)) == 0 );
}

TEST_CASE("get_axis_pivot_offset should return 0 for empty axes", "[strided_axis]")
{
    REQUIRE( get_axis_pivot_offset(strided_axis(0, 0)) == 0 );
    REQUIRE( get_axis_pivot_offset(strided_axis(0, 1)) == 0 );
    REQUIRE( get_axis_pivot_offset(strided_axis(0, -1)) == 0 );
}

TEST_CASE("get_axis_pivot_offset should return the last offset for reversed axes", "[strided_axis]")
{
    REQUIRE( get_axis_pivot_offset(strided_axis(18, -1)) == 17 );
    REQUIRE( get_axis_pivot_offset(strided_axis(11, -10)) == 100 );
}

TEST_CASE("broadcast with two axes of equal extents should succeed", "[strided_axis]")
{
    strided_axis axis0(3, 1);
    strided_axis axis1(3, 2);
    REQUIRE( broadcast(axis0, axis1) );
    REQUIRE( axis0 == strided_axis(3, 1) );
    REQUIRE( axis1 == strided_axis(3, 2) );
}

TEST_CASE("broadcast with two compatible axes should succeed", "[strided_axis]")
{
    strided_axis axis0(3, 1);
    strided_axis axis1(1, 2);

    SECTION ("Ordered")
    {
        REQUIRE( broadcast(axis0, axis1) );
    }

    SECTION ("Reversed")
    {
        REQUIRE( broadcast(axis1, axis0) );
    }

    REQUIRE( axis0 == strided_axis(3, 1) );
    REQUIRE( axis1 == strided_axis(3, 0) );
}

TEST_CASE("broadcast with two incompatible axes should fail", "[strided_axis]")
{
    strided_axis axis0(3, 1);
    strided_axis axis1(2, 2);
    REQUIRE( !broadcast(axis1, axis0) );
}

TEST_CASE("apply_index should increment the offset as expected", "[strided_axis]")
{
    const std::ptrdiff_t initial_offset = 24;
    const strided_axis axis(16, -5);
    const std::size_t index = 3;

    auto offset = initial_offset;
    apply_index(axis, offset, index);
    REQUIRE( offset == initial_offset + index*axis.get_stride() );
}

TEST_CASE("apply_slice increment the offset and modify the axis", "[strided_axis]")
{
    const std::ptrdiff_t initial_offset = 24;
    const strided_axis initial_axis(16, -5);
    const slice<std::size_t, std::size_t, std::ptrdiff_t> slice(2, 5, 3);

    auto axis = initial_axis;
    auto offset = initial_offset;
    apply_slice(axis, offset, slice);
    REQUIRE( offset == initial_offset + slice.get_start()*initial_axis.get_stride() );
    REQUIRE( axis.get_extent() == slice.get_count() );
    REQUIRE( axis.get_stride() == slice.get_step()*initial_axis.get_stride() );
}

