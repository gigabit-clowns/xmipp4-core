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
 * @file test_memory_layout.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for multidimensional/strided_layout_utils.hpp
 * @date 2023-09-25
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/strided_layout_utils.hpp>

#include <tuple>
#include <vector>

using namespace xmipp4::multidimensional;

XMIPP4_INLINE_CONSTEXPR
bool lexicographic_axis_compare(const axis_descriptor &lhs, 
                                const axis_descriptor &rhs) noexcept
{
    const auto lhs_tuple = std::make_tuple(lhs.get_extent(), lhs.get_stride());
    const auto rhs_tuple = std::make_tuple(rhs.get_extent(), rhs.get_stride());
    return lhs_tuple < rhs_tuple;
}

TEST_CASE("find max and min stride", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 8),
        axis_descriptor(2, 64),
        axis_descriptor(4, -128),
        axis_descriptor(4, 16),
        axis_descriptor(2, 1),
        axis_descriptor(4, -2)
    };

    REQUIRE(find_max_stride(layout.cbegin(), layout.cend()) == std::next(layout.cbegin(), 2));
    REQUIRE(find_min_stride(layout.cbegin(), layout.cend()) == std::next(layout.cbegin(), 4));
}

TEST_CASE("find first significant axis", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 8),
        axis_descriptor(2, 64),
        axis_descriptor(4, -128),
        axis_descriptor(4, 16),
        axis_descriptor(1, -1),
        axis_descriptor(2, 1),
        axis_descriptor(4, 0),
        axis_descriptor(1, -16),
        axis_descriptor(4, -2)
    };

    std::sort(layout.begin(), layout.end(), lexicographic_axis_compare);
    while(std::next_permutation(layout.begin(), layout.end(), lexicographic_axis_compare))
    {
        auto ite = find_first_significant_axis(layout.cbegin(), layout.cend());
        REQUIRE( *ite == axis_descriptor(2, 1) );
    }
}

TEST_CASE("find next significant axis", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 8),
        axis_descriptor(2, 64),
        axis_descriptor(4, -128),
        axis_descriptor(4, 16),
        axis_descriptor(4, 0),
        axis_descriptor(1, -16),
        axis_descriptor(2, 1),
        axis_descriptor(4, -2)
    };

    std::sort(layout.begin(), layout.end(), lexicographic_axis_compare);
    while(std::next_permutation(layout.begin(), layout.end(), lexicographic_axis_compare))
    {
        auto ite = find_first_significant_axis(layout.cbegin(), layout.cend());
        REQUIRE( *ite == axis_descriptor(2, 1) );
        ite = find_next_significant_axis(ite, layout.cbegin(), layout.cend());
        REQUIRE( *ite == axis_descriptor(4, -2) );
        ite = find_next_significant_axis(ite, layout.cbegin(), layout.cend());
        REQUIRE( *ite == axis_descriptor(2, 8) );
        ite = find_next_significant_axis(ite, layout.cbegin(), layout.cend());
        REQUIRE( *ite == axis_descriptor(4, 16) );
        ite = find_next_significant_axis(ite, layout.cbegin(), layout.cend());
        REQUIRE( *ite == axis_descriptor(2, 64) );
        ite = find_next_significant_axis(ite, layout.cbegin(), layout.cend());
        REQUIRE( *ite == axis_descriptor(4, -128) );
        ite = find_next_significant_axis(ite, layout.cbegin(), layout.cend());
        REQUIRE( ite == layout.cend() );
    }
}

TEST_CASE("pack layout", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 0),
        axis_descriptor(4, 2),
        axis_descriptor(2, 8),
        axis_descriptor(10, 0),
        //---------------------
        axis_descriptor(4, -32),
        axis_descriptor(1, -128),
        axis_descriptor(3, 128),
        //---------------------
        axis_descriptor(9, -512),
        axis_descriptor(2, 0),
    };

    std::vector<axis_descriptor> packed;
    std::ptrdiff_t offset;

    std::sort(layout.begin(), layout.end(), lexicographic_axis_compare);
    while(std::next_permutation(layout.begin(), layout.end(), lexicographic_axis_compare))
    {
        // Clear output
        packed.clear();
        offset = 0;

        // Pack
        pack_layout(
            layout.cbegin(), layout.cend(),
            std::back_inserter(packed),
            offset
        );

        // Test
        REQUIRE( packed.size() == 3 );
        REQUIRE( packed[0] == axis_descriptor(8, 2) );
        REQUIRE( packed[1] == axis_descriptor(12, 32) );
        REQUIRE( packed[2] == axis_descriptor(9, 512) );
        REQUIRE( offset == -(8*512+3*32) );
    }
}

TEST_CASE("compute contiguous axis strides", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        make_contiguous_axis(2),
        make_contiguous_axis(3),
        make_contiguous_axis(8),
        make_contiguous_axis(5),
        make_contiguous_axis(1),
        make_contiguous_axis(9)
    };

    SECTION("column major")
    {
        const auto volume = compute_contiguous_axis_strides(layout.begin(), layout.end(), column_major());
        REQUIRE(layout[0].get_stride() == 1);
        REQUIRE(layout[1].get_stride() == 2);
        REQUIRE(layout[2].get_stride() == 6);
        REQUIRE(layout[3].get_stride() == 48);
        REQUIRE(layout[4].get_stride() == 240);
        REQUIRE(layout[5].get_stride() == 240);
        REQUIRE(volume == 2160);
    }

    SECTION("row major")
    {
        const auto volume = compute_contiguous_axis_strides(layout.begin(), layout.end(), row_major());
        REQUIRE(layout[5].get_stride() == 1);
        REQUIRE(layout[4].get_stride() == 9);
        REQUIRE(layout[3].get_stride() == 9);
        REQUIRE(layout[2].get_stride() == 45);
        REQUIRE(layout[1].get_stride() == 360);
        REQUIRE(layout[0].get_stride() == 1080);
        REQUIRE(volume == 2160);
    }
}

TEST_CASE("compute layout buffer size", "memory_layout")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 0),
        axis_descriptor(2, -1),
        axis_descriptor(4, 2),
        axis_descriptor(2, 8),
        axis_descriptor(4, -16),
        axis_descriptor(10, 0),
        axis_descriptor(1, -64),
        axis_descriptor(2, 64),
        axis_descriptor(4, 128),
        axis_descriptor(2, 0),
    };

    SECTION("normal")
    {
        const auto size = compute_layout_buffer_size(layout.cbegin(), layout.cend());
        REQUIRE(size == 512);
    }

    SECTION("non contiguous")
    {
        layout[1] = axis_descriptor(1, 2);
        const auto size = compute_layout_buffer_size(layout.cbegin(), layout.cend());
        REQUIRE(size == 511); // Last element no longer required
    }

    SECTION("zero sized axis")
    {
        layout[0] = axis_descriptor(0, 1);
        const auto size = compute_layout_buffer_size(layout.cbegin(), layout.cend());
        REQUIRE(size == 0);
    }

    SECTION("empty")
    {
        layout.clear();
        const auto size = compute_layout_buffer_size(layout.cbegin(), layout.cend());
        REQUIRE(size == 1);
    }
}

TEST_CASE("transpose layout", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 8),
        axis_descriptor(2, 64),
        axis_descriptor(4, 128),
        axis_descriptor(4, 16),
        axis_descriptor(2, 1),
        axis_descriptor(4, 2)
    };

    // Transpose the layout out of place
    std::vector<axis_descriptor> transposed_layout;
    transpose_layout(layout.cbegin(), layout.cend(), std::back_inserter(transposed_layout));

    REQUIRE(std::equal(layout.crbegin(), layout.crend(), transposed_layout.cbegin(), transposed_layout.cend()));

    // Transpose the layout in-place
    transpose_layout(layout.begin(), layout.end());
    REQUIRE(std::equal(layout.cbegin(), layout.cend(), transposed_layout.cbegin(), transposed_layout.cend()));
}

TEST_CASE("squeeze layout", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(4, 1),
        axis_descriptor(1, 4),
        axis_descriptor(4, 4),
        axis_descriptor(1, 16),
        axis_descriptor(4, 16),
    };

    SECTION("in-place")
    {
        const std::vector<axis_descriptor> squeezed_layout = {
            axis_descriptor(4, 1),
            axis_descriptor(4, 4),
            axis_descriptor(4, 16),

        };
        layout.erase(squeeze_layout(layout.begin(), layout.end()), layout.end());
        REQUIRE(layout == squeezed_layout);
    }
    SECTION("out of place")
    {
        std::vector<axis_descriptor> squeezed_layout;
        squeeze_layout(layout.cbegin(), layout.cend(), std::back_inserter(squeezed_layout));
        layout.erase(squeeze_layout(layout.begin(), layout.end()), layout.end());
        REQUIRE(layout == squeezed_layout);
    }
}

TEST_CASE("broadcast layouts", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 0),
        axis_descriptor(2, 1),
        axis_descriptor(1, 2),
        axis_descriptor(4, 2),
        axis_descriptor(3, 8),
        axis_descriptor(2, 24),
        axis_descriptor(1, 0),
    };

    SECTION("itself")
    {
        REQUIRE( broadcast_layouts(layout.size(), layout.begin(), layout.begin()) );
    }

    SECTION("broadcasting axes")
    {
        auto layout2 = layout;
        layout2[3].set_extent(1);
        REQUIRE( broadcast_layouts(layout.size(), layout.begin(), layout2.begin()) );
        REQUIRE( layout2[3].get_extent() == 4 );
        REQUIRE( layout2[3].get_stride() == 0 );
    }

    SECTION("unable to broadcast axes")
    {
        auto layout2 = layout;
        layout2[3].set_extent(2);
        REQUIRE( !broadcast_layouts(layout.size(), layout.begin(), layout2.begin()) );
    }
}
