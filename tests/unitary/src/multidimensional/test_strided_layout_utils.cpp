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

TEST_CASE("find first significant axis", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(4, 0),
        axis_descriptor(1, 2),
        axis_descriptor(3, -2),
        axis_descriptor(2, 1),
    };

    REQUIRE(find_first_significant_axis(layout.cbegin(), layout.cend()) == std::next(layout.cbegin(), 2));
}

TEST_CASE("find major axis", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 8),
        axis_descriptor(2, 64),
        axis_descriptor(4, -128),
        axis_descriptor(4, 0),
        axis_descriptor(4, 16),
        axis_descriptor(2, 1),
        axis_descriptor(4, -2)
    };

    REQUIRE(find_major_axis(layout.cbegin(), layout.cend()) == std::next(layout.cbegin(), 5));
}

TEST_CASE("find minor axis", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 8),
        axis_descriptor(2, 64),
        axis_descriptor(1, 512),
        axis_descriptor(4, -128),
        axis_descriptor(4, 0),
        axis_descriptor(4, 16),
        axis_descriptor(2, 1),
        axis_descriptor(4, -2)
    };

    REQUIRE(find_minor_axis(layout.cbegin(), layout.cend()) == std::next(layout.cbegin(), 3));
}

TEST_CASE("sort layout", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(1, 2),
        axis_descriptor(4, 0),
        axis_descriptor(1, 512),
        axis_descriptor(2, 1),
        axis_descriptor(2, 8),
        axis_descriptor(4, 0),
        axis_descriptor(4, -2),
        axis_descriptor(4, -128),
        axis_descriptor(2, 64),
        axis_descriptor(3, 0),
    };

    SECTION ("column major")
    {
        REQUIRE( !check_layout_order(layout.cbegin(), layout.cend(), column_major()) );
        sort_layout_inplace(layout.begin(), layout.end(), column_major());
        REQUIRE( check_layout_order(layout.cbegin(), layout.cend(), column_major()) );
    }

    SECTION ("row major")
    {
        REQUIRE( !check_layout_order(layout.cbegin(), layout.cend(), row_major()) );
        sort_layout_inplace(layout.begin(), layout.end(), row_major());
        REQUIRE( check_layout_order(layout.cbegin(), layout.cend(), row_major()) );
    }
}

TEST_CASE("check layout order", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 1),
        axis_descriptor(1, 2),
        axis_descriptor(4, -2),
        axis_descriptor(1, 2),
        axis_descriptor(4, 0),
        axis_descriptor(2, 8),
        axis_descriptor(2, 64),
        axis_descriptor(4, 0),
        axis_descriptor(4, -128),
        axis_descriptor(3, 0),
        axis_descriptor(1, 512),
    };

    REQUIRE( check_layout_order(layout.cbegin(), layout.cend(), row_major()) );
    REQUIRE( check_layout_order(layout.crbegin(), layout.crend(), column_major()) );

    std::swap(layout[0], layout[2]);

    REQUIRE( !check_layout_order(layout.cbegin(), layout.cend(), row_major()) );
    REQUIRE( !check_layout_order(layout.crbegin(), layout.crend(), column_major()) );
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

    std::vector<axis_descriptor> packed = layout;
    std::ptrdiff_t offset = 0;

    SECTION("row major")
    {
        std::sort(packed.begin(), packed.end(), compare_strides_less);
        auto ite = pack_layout_inplace(
            packed.begin(), packed.end(),
            offset,
            row_major()
        );
        packed.erase(ite, packed.end());

        REQUIRE( packed.size() == 3 );
        REQUIRE( packed[0] == axis_descriptor(8, 2) );
        REQUIRE( packed[1] == axis_descriptor(12, 32) );
        REQUIRE( packed[2] == axis_descriptor(9, 512) );
        REQUIRE( offset == -(8*512+3*32) );
    }

    SECTION("column major")
    {
        std::sort(packed.rbegin(), packed.rend(), compare_strides_less);
        auto ite = pack_layout_inplace(
            packed.begin(), packed.end(),
            offset,
            column_major()
        );
        packed.erase(ite, packed.end());

        REQUIRE( packed.size() == 3 );
        REQUIRE( packed[0] == axis_descriptor(9, 512) );
        REQUIRE( packed[1] == axis_descriptor(12, 32) );
        REQUIRE( packed[2] == axis_descriptor(8, 2) );
        REQUIRE( offset == -(8*512+3*32) );
    }
}

TEST_CASE("is contiguous layout", "[memory_layout]")
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

    SECTION("contiguous")
    {
        REQUIRE( is_contiguous_layout(layout.cbegin(), layout.cend()) );
    }
    SECTION("first axis has not unit stride")
    {
        layout[1] = axis_descriptor(1, 2);

        REQUIRE( !is_contiguous_layout(layout.cbegin(), layout.cend()) );
    }
    SECTION("not packed")
    {
        layout[2] = axis_descriptor(2, -4);

        REQUIRE( !is_contiguous_layout(layout.cbegin(), layout.cend()) );
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
    transpose_layout_inplace(layout.begin(), layout.end());
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
        layout.erase(squeeze_layout_inplace(layout.begin(), layout.end()), layout.end());
        REQUIRE(layout == squeezed_layout);
    }
    SECTION("out of place")
    {
        std::vector<axis_descriptor> squeezed_layout;
        squeeze_layout(layout.cbegin(), layout.cend(), std::back_inserter(squeezed_layout));
        layout.erase(squeeze_layout_inplace(layout.begin(), layout.end()), layout.end());
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
