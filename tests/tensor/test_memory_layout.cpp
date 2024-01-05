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

#include <xmipp4/tensor/memory_layout.hpp>

#include <vector>

using namespace xmipp4;


TEST_CASE( "check axis pair ordering", "[memory_layout]" ) 
{
    const axis_descriptor major(12, 1);
    const axis_descriptor minor(12, 12);

    SECTION("column major")
    {
        REQUIRE(check_axis_order(major, minor, column_major));
        REQUIRE(!check_axis_order(major, minor, row_major));
    }
    SECTION("row major")
    {
        REQUIRE(!check_axis_order(minor, major, column_major));
        REQUIRE(check_axis_order(minor, major, row_major));
    }
}

TEST_CASE( "check axis pair overlap", "[memory_layout]" ) 
{
    SECTION("overlapping")
    {
        const axis_descriptor major(12, 1);
        const axis_descriptor minor(12, 6);
        REQUIRE(check_axis_overlap(major, minor));
    }
    SECTION("non overlapping")
    {
        const axis_descriptor major(12, 1);
        const axis_descriptor minor(12, 12);
        REQUIRE(!check_axis_overlap(major, minor));
    }
}

TEST_CASE( "check axis pair regularity", "[memory_layout]" ) 
{
    SECTION("regular")
    {
        const axis_descriptor major(12, 1);
        const axis_descriptor minor(12, 12);
        REQUIRE(is_regular(major, minor));
    }
    SECTION("non regular")
    {
        const axis_descriptor major(12, 1);
        const axis_descriptor minor(12, 14);
        REQUIRE(!is_regular(major, minor));
    }
}

TEST_CASE ("check layout ordering", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 1),
        axis_descriptor(4, 2),
        axis_descriptor(2, 8),
        axis_descriptor(4, 16),
        axis_descriptor(2, 64),
        axis_descriptor(4, 256)
    };

    SECTION("ordered")
    {
        REQUIRE(check_layout_order(layout.cbegin(), layout.cend(), column_major));
        REQUIRE(!check_layout_order(layout.cbegin(), layout.cend(), row_major));
        REQUIRE(!check_layout_order(layout.crbegin(), layout.crend(), column_major));
        REQUIRE(check_layout_order(layout.crbegin(), layout.crend(), row_major));
    }
    SECTION("disordered")
    {
        std::swap(layout[2], layout[3]);
        REQUIRE(!check_layout_order(layout.cbegin(), layout.cend(), column_major));
        REQUIRE(!check_layout_order(layout.cbegin(), layout.cend(), row_major));
        REQUIRE(!check_layout_order(layout.crbegin(), layout.crend(), column_major));
        REQUIRE(!check_layout_order(layout.crbegin(), layout.crend(), row_major));
    }
}

TEST_CASE("check layout overlap")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 1),
        axis_descriptor(4, 2),
        axis_descriptor(2, 8),
        axis_descriptor(4, 16),
        axis_descriptor(2, 64),
        axis_descriptor(4, 128)
    };

    SECTION("non overlapping")
    {
        REQUIRE(!check_layout_overlap(layout.cbegin(), layout.cend(), column_major));
        REQUIRE(!check_layout_overlap(layout.crbegin(), layout.crend(), row_major));
    }
    SECTION("overlapping")
    {
        layout[0].set_count(3);
        REQUIRE(check_layout_overlap(layout.cbegin(), layout.cend(), column_major));
        REQUIRE(check_layout_overlap(layout.crbegin(), layout.crend(), row_major));
    }
}

TEST_CASE("check contiguous layout", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 1),
        axis_descriptor(4, 2),
        axis_descriptor(2, 8),
        axis_descriptor(4, 16),
        axis_descriptor(2, 64),
        axis_descriptor(4, 128)
    };

    SECTION("contiguous")
    {
        REQUIRE(is_contiguous_layout(layout.cbegin(), layout.cend(), column_major));
        REQUIRE(is_contiguous_layout(layout.crbegin(), layout.crend(), row_major));
    }
    SECTION("first axis has not unit step")
    {
        layout[0] = axis_descriptor(1, 2);
        REQUIRE(!is_contiguous_layout(layout.cbegin(), layout.cend(), column_major));
        REQUIRE(!is_contiguous_layout(layout.crbegin(), layout.crend(), row_major));
    }
    SECTION("disordered")
    {
        std::swap(layout[2], layout[3]);
        REQUIRE(!is_contiguous_layout(layout.cbegin(), layout.cend(), column_major));
        REQUIRE(!is_contiguous_layout(layout.crbegin(), layout.crend(), row_major));
    }
}

TEST_CASE("compute contiguous axis steps")
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
        const auto volume = compute_contiguous_axis_steps(layout.begin(), layout.end(), column_major);
        REQUIRE(layout[0].get_step() == 1);
        REQUIRE(layout[1].get_step() == 2);
        REQUIRE(layout[2].get_step() == 6);
        REQUIRE(layout[3].get_step() == 48);
        REQUIRE(layout[4].get_step() == 240);
        REQUIRE(layout[5].get_step() == 240);
        REQUIRE(volume == 2160);
    }

    SECTION("row major")
    {
        const auto volume = compute_contiguous_axis_steps(layout.begin(), layout.end(), row_major);
        REQUIRE(layout[5].get_step() == 1);
        REQUIRE(layout[4].get_step() == 9);
        REQUIRE(layout[3].get_step() == 9);
        REQUIRE(layout[2].get_step() == 45);
        REQUIRE(layout[1].get_step() == 360);
        REQUIRE(layout[0].get_step() == 1080);
        REQUIRE(volume == 2160);
    }
}

TEST_CASE("find max and min step", "[memory_layout]")
{
    std::vector<axis_descriptor> layout = {
        axis_descriptor(2, 8),
        axis_descriptor(2, 64),
        axis_descriptor(4, 128),
        axis_descriptor(4, 16),
        axis_descriptor(2, 1),
        axis_descriptor(4, 2)
    };

    REQUIRE(find_max_step(layout.cbegin(), layout.cend()) == std::next(layout.cbegin(), 2));
    REQUIRE(find_min_step(layout.cbegin(), layout.cend()) == std::next(layout.cbegin(), 4));
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

TEST_CASE("flatten contiguous layout", "[memory_layout]")
{
    SECTION("column major")
    {
        std::vector<axis_descriptor> layout = {
            axis_descriptor(4, 2),
            axis_descriptor(2, 8),
            axis_descriptor(4, 16),
            axis_descriptor(2, 64),
            axis_descriptor(4, 128),
        };

        REQUIRE(flatten_regular_layout(layout.cbegin(), layout.cend(), column_major) == axis_descriptor(256, 2));
    }
    SECTION("row major")
    {
        std::vector<axis_descriptor> layout = {
            axis_descriptor(4, 128),
            axis_descriptor(2, 64),
            axis_descriptor(4, 16),
            axis_descriptor(2, 8),
            axis_descriptor(4, 2),
        };

        REQUIRE(flatten_regular_layout(layout.cbegin(), layout.cend(), row_major) == axis_descriptor(256, 2));
    }
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