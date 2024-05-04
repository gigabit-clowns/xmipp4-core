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
 * @brief Tests for tensor/memory_layout.hpp
 * @date 2023-09-25
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/strided_layout_utils.hpp>

#include <vector>

using namespace xmipp4::multidimensional;

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

TEST_CASE("find next axis", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 8), //0
        axis_descriptor(2, 64), //1
        axis_descriptor(4, -128), //2
        axis_descriptor(4, 16), //3
        axis_descriptor(4, 0), //4
        axis_descriptor(1, -16), //5
        axis_descriptor(2, 1), //6
        axis_descriptor(4, -2) //7
    };

    auto ite = find_min_stride(layout.cbegin(), layout.cend());
    REQUIRE(ite == std::next(layout.cbegin(), 4));
    ite = find_next_axis(ite, layout.cbegin(), layout.cend());
    REQUIRE(ite == std::next(layout.cbegin(), 6));
    ite = find_next_axis(ite, layout.cbegin(), layout.cend());
    REQUIRE(ite == std::next(layout.cbegin(), 7));
    ite = find_next_axis(ite, layout.cbegin(), layout.cend());
    REQUIRE(ite == std::next(layout.cbegin(), 0));
    ite = find_next_axis(ite, layout.cbegin(), layout.cend());
    REQUIRE(ite == std::next(layout.cbegin(), 3));
    ite = find_next_axis(ite, layout.cbegin(), layout.cend());
    REQUIRE(ite == std::next(layout.cbegin(), 5));
    ite = find_next_axis(ite, layout.cbegin(), layout.cend());
    REQUIRE(ite == std::next(layout.cbegin(), 1));
    ite = find_next_axis(ite, layout.cbegin(), layout.cend());
    REQUIRE(ite == std::next(layout.cbegin(), 2));
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
        axis_descriptor(2, 128),
        //---------------------
        axis_descriptor(9, -512),
        axis_descriptor(2, 0),
    };

    std::vector<axis_descriptor> packed;
    std::ptrdiff_t offset = 0;

    SECTION("row major")
    {
        pack_layout(
            layout.cbegin(), layout.cend(),
            std::back_inserter(packed),
            offset,
            row_major()
        );

        REQUIRE( packed.size() == 3 );
        REQUIRE( packed[0] == axis_descriptor(8, 2) );
        REQUIRE( packed[1] == axis_descriptor(8, 32) );
        REQUIRE( packed[2] == axis_descriptor(9, 512) );
        REQUIRE( offset == -(8*512+3*32) );
    }
    
    SECTION("column major")
    {
        pack_layout(
            layout.crbegin(), layout.crend(),
            std::back_inserter(packed),
            offset,
            column_major()
        );

        REQUIRE( packed.size() == 3 );
        REQUIRE( packed[0] == axis_descriptor(9, 512) );
        REQUIRE( packed[1] == axis_descriptor(8, 32) );
        REQUIRE( packed[2] == axis_descriptor(8, 2) );
        REQUIRE( offset == -(8*512+3*32) );
    }
}

TEST_CASE("check packed layout", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 0),
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
        REQUIRE(is_packed_layout(layout.cbegin(), layout.cend(), row_major()));
        REQUIRE(is_packed_layout(layout.crbegin(), layout.crend(), column_major()));
    }
    SECTION("not packed")
    {
        layout[2] = axis_descriptor(2, -4);
        REQUIRE(!is_packed_layout(layout.cbegin(), layout.cend(), row_major()));
        REQUIRE(!is_packed_layout(layout.crbegin(), layout.crend(), column_major()));
    }
}

TEST_CASE("check contiguous layout", "[memory_layout]")
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
        REQUIRE(is_contiguous_layout(layout.cbegin(), layout.cend(), row_major()));
        REQUIRE(is_contiguous_layout(layout.crbegin(), layout.crend(), column_major()));
    }
    SECTION("first axis has not unit stride")
    {
        layout[1] = axis_descriptor(1, 2);
        REQUIRE(!is_contiguous_layout(layout.cbegin(), layout.cend(), row_major()));
        REQUIRE(!is_contiguous_layout(layout.crbegin(), layout.crend(), column_major()));
    }
    SECTION("not packed")
    {
        layout[2] = axis_descriptor(2, -4);
        REQUIRE(!is_contiguous_layout(layout.cbegin(), layout.cend(), row_major()));
        REQUIRE(!is_contiguous_layout(layout.crbegin(), layout.crend(), column_major()));
    }
}

TEST_CASE("compute contiguous axis strides", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2),
        axis_descriptor(3),
        axis_descriptor(8),
        axis_descriptor(5),
        axis_descriptor(1),
        axis_descriptor(9)
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
