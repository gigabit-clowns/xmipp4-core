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
#include <utility>

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

TEST_CASE("broadcast with equal extents should succeed and leave axes unmodified", "[strided_axis]")
{
    const strided_axis initial_axis1(123, 345);
    const strided_axis initial_axis2(123, 654);

    auto axis1 = initial_axis1;
    auto axis2 = initial_axis2;
    REQUIRE( broadcast(axis1, axis2) );
    REQUIRE(axis1 == initial_axis1 );
    REQUIRE(axis2 == initial_axis2 );
}

TEST_CASE("broadcast with left axis of extent 1 should broadcast it")
{
    const strided_axis initial_axis1(1, 345);
    const strided_axis initial_axis2(123, 654);

    auto axis1 = initial_axis1;
    auto axis2 = initial_axis2;
    REQUIRE( broadcast(axis1, axis2) );
    REQUIRE(axis1 == make_phantom_axis(axis2.get_extent()) );
    REQUIRE(axis2 == initial_axis2 );
}

TEST_CASE("broadcast with right axis of extent 1 should broadcast it")
{
    const strided_axis initial_axis1(123, 345);
    const strided_axis initial_axis2(1, 654);

    auto axis1 = initial_axis1;
    auto axis2 = initial_axis2;
    REQUIRE( broadcast(axis1, axis2) );
    REQUIRE(axis1 == initial_axis1 );
    REQUIRE(axis2 == make_phantom_axis(axis1.get_extent()) );
}

TEST_CASE("broadcast with unequal extents should fail and leave axes unmodified", "[strided_axis]")
{
    const std::array<std::pair<std::size_t, std::size_t>, 4> test_cases = {
        std::pair<std::size_t, std::size_t>(76, 0),
        std::pair<std::size_t, std::size_t>(0, 3),
        std::pair<std::size_t, std::size_t>(2, 4),
        std::pair<std::size_t, std::size_t>(252, 308)
    };

    for (const auto &test_case : test_cases)
    {
        const strided_axis initial_axis1(test_case.first, 345);
        const strided_axis initial_axis2(test_case.second, 654);

        auto axis1 = initial_axis1;
        auto axis2 = initial_axis2;
        REQUIRE( !broadcast(axis1, axis2) );
        REQUIRE(axis1 == initial_axis1 );
        REQUIRE(axis2 == initial_axis2 );
    }
}

TEST_CASE("broadcast_to with equal extents should succeed and leave axis unmodified", "[strided_axis]")
{
    const std::size_t extent = 876;
    const auto initial_axis = make_contiguous_axis(extent);

    auto axis = initial_axis;
    REQUIRE( broadcast_to(axis, extent) );
    REQUIRE( axis == initial_axis );
}

TEST_CASE("broadcast_to with an axis of extent 1 should broadcast it")
{
    const std::size_t extent = 565;

    auto axis = make_contiguous_axis();
    REQUIRE( broadcast_to(axis, extent) );
    REQUIRE( axis == make_phantom_axis(extent) );
}

TEST_CASE("broadcast_to with unequal extents should fail and leave axis unmodified", "[strided_axis]")
{
    const std::array<std::pair<std::size_t, std::size_t>, 4> test_cases = {
        std::pair<std::size_t, std::size_t>(0, 1),
        std::pair<std::size_t, std::size_t>(43, 0),
        std::pair<std::size_t, std::size_t>(12, 1),
        std::pair<std::size_t, std::size_t>(654, 345)
    };

    for (const auto &test_case : test_cases)
    {
        const auto initial_axis = make_contiguous_axis(test_case.first);
        const auto extent = test_case.second;

        auto axis = initial_axis;
        REQUIRE( !broadcast_to(axis, extent) );
        REQUIRE( axis == initial_axis );
    }
}

TEST_CASE("broadcast_dry with equal extents should succeed and leave extent unmodified", "[strided_axis]")
{
    const std::size_t initial_extent = 654;
    const auto axis = make_contiguous_axis(initial_extent);

    auto extent = initial_extent;
    REQUIRE( broadcast_dry(axis, extent) );
    REQUIRE( extent == initial_extent );
}

TEST_CASE("broadcast_dry with an extent 1 should broadcast it")
{
    const auto axis = make_contiguous_axis(1234);

    std::size_t extent = 1;
    REQUIRE( broadcast_dry(axis, extent) );
    REQUIRE( extent == axis.get_extent() );
}

TEST_CASE("broadcast_dry with unequal extents should fail and leave extent unmodified", "[strided_axis]")
{
    const std::array<std::pair<std::size_t, std::size_t>, 4> test_cases = {
        std::pair<std::size_t, std::size_t>(0, 1),
        std::pair<std::size_t, std::size_t>(76, 0),
        std::pair<std::size_t, std::size_t>(13, 1),
        std::pair<std::size_t, std::size_t>(252, 308)
    };

    for (const auto &test_case : test_cases)
    {
        const auto initial_extent = test_case.first;
        const auto axis = make_contiguous_axis(test_case.second);

        auto extent = initial_extent;
        REQUIRE( !broadcast_dry(axis, extent) );
        REQUIRE( extent == initial_extent );
    }
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

TEST_CASE("apply_slice should increment the offset and modify the axis", "[strided_axis]")
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
