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
 * @file test_strided_layout.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for multidimensional/strided_layout.hpp
 * @date 2024-07-02
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include <algorithm>

using namespace xmipp4::multidimensional;

TEST_CASE("construct a strided_layout from its components", "[strided_layout]")
{
    const std::vector<strided_axis> axes = 
    {
        strided_axis(123, 135),
        strided_axis(543, 432),
        strided_axis(305, 543),
        strided_axis(234, 432),
    };
    const std::size_t offset = 54634552;

    const strided_layout layout(std::vector<strided_axis>(axes), offset);

    REQUIRE( layout.get_offset() == offset );   
    REQUIRE( layout.get_rank() == axes.size() );
    
    strided_axis axis;
    for (std::size_t i = 0; i < axes.size(); ++i)
    {
        REQUIRE( layout.get_axis(i, axis) );
        REQUIRE( axis.get_extent() == axes[i].get_extent() );
        REQUIRE( axis.get_stride() == axes[i].get_stride() );
    }
}

TEST_CASE("construct a strided_layout from its components as a pointer", "[strided_layout]")
{
    const std::vector<strided_axis> axes = 
    {
        strided_axis(123, 135),
        strided_axis(543, 432),
        strided_axis(305, 543),
        strided_axis(234, 432),
    };
    const std::size_t offset = 54634552;

    const strided_layout layout(axes.data(), axes.size(), offset);

    REQUIRE( layout.get_offset() == offset );   
    REQUIRE( layout.get_rank() == axes.size() );
    
    strided_axis axis;
    for (std::size_t i = 0; i < axes.size(); ++i)
    {
        REQUIRE( layout.get_axis(i, axis) );
        REQUIRE( axis.get_extent() == axes[i].get_extent() );
        REQUIRE( axis.get_stride() == axes[i].get_stride() );
    }
}

TEST_CASE("construct a column-major strided_layout from extents", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        16,
        1,
        5,
        6
    };
    const std::size_t offset = 40;

    const strided_layout layout(extents.data(), extents.size(), offset);

    REQUIRE( layout.get_offset() == offset );   
    REQUIRE( layout.get_rank() == extents.size() );
    
    const std::vector<std::ptrdiff_t> expected_strides = 
    {
        1*5*6,
        5*6,
        6,
        1,
    };

    strided_axis axis;
    for (std::size_t i = 0; i < extents.size(); ++i)
    {
        REQUIRE( layout.get_axis(i, axis) );
        REQUIRE( axis.get_extent() == extents[i] );
        REQUIRE( axis.get_stride() == expected_strides[i] );
    }
}

TEST_CASE("construct a strided_layout from separate lists", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 7631;

    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(), 
        offset
    );

    REQUIRE( layout.get_offset() == offset );
    REQUIRE( layout.get_rank() == extents.size() );
    
    strided_axis axis;
    for (std::size_t i = 0; i < extents.size(); ++i)
    {
        REQUIRE( layout.get_axis(i, axis) );
        REQUIRE( axis.get_extent() == extents[i] );
        REQUIRE( axis.get_stride() == strides[i] );
    }
}

TEST_CASE("get_axes in strided_layout should provide the correct contents", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };

    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size()
    );

    const auto count = layout.get_rank();
    std::vector<std::size_t> read_extents(count);
    std::vector<std::ptrdiff_t> read_strides(count);
    const auto read = 
        layout.get_axes(read_extents.data(), read_strides.data(), count);

    REQUIRE( read == count );
    REQUIRE( read_extents == extents );
    REQUIRE( read_strides == strides );
}

TEST_CASE("get_axes in strided_layout should tolerate a null stride pointer", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };

    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size()
    );

    const auto count = layout.get_rank();
    std::vector<std::size_t> read_extents(count);
    const auto read = 
        layout.get_axes(read_extents.data(), nullptr, count);

    REQUIRE( read == count );
    REQUIRE( read_extents == extents );
}

TEST_CASE("get_axes in strided_layout should tolerate null extent pointer", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size()
    );

    const auto count = layout.get_rank();
    std::vector<std::ptrdiff_t> read_strides(count);
    const auto read = 
        layout.get_axes(nullptr, read_strides.data(), count);

    REQUIRE( read == count );
    REQUIRE( read_strides == strides );
}

TEST_CASE("get_axes in strided_layout should not overflow the buffer", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };

    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size()
    );

    const auto count = 2;
    std::vector<std::size_t> read_extents(count);
    std::vector<std::ptrdiff_t> read_strides(count);
    const auto read = 
        layout.get_axes(read_extents.data(), read_strides.data(), count);

    REQUIRE( read == count );
    for (std::size_t i = 0; i < count; ++i)
    {
        REQUIRE(read_extents[i] == extents[i]);
        REQUIRE(read_strides[i] == strides[i]);
    }
}

TEST_CASE("get_axes axis in strided_layout should succeed for all valid indices", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size()
    );

    const auto count = layout.get_rank();
    strided_axis axis;
    for (std::size_t i = 0; i < count; ++i)
    {
        REQUIRE( layout.get_axis(i, axis) );
        REQUIRE( axis.get_extent() == extents[i]);
        REQUIRE( axis.get_stride() == strides[i]);
    }
}

TEST_CASE("get_axes axis in strided_layout should fail for out of range indices", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size()
    );

    const auto count = layout.get_rank();
    const strided_axis initial_axis(245, 2435);
    auto axis = initial_axis;
    for (std::size_t i = count; i < count+128; ++i)
    {
        REQUIRE( !layout.get_axis(i, axis) );
        REQUIRE( axis == initial_axis );
    }
}

TEST_CASE("compute_storage_requirement in strided_layout should return the last index + 1", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        32, 16, 12, 6
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        1, 32, 512, 8192
    };
    const auto offset = 20;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    const auto expected_size = 31 + 15*32 + 11*512 + 5*8192 + offset + 1;
    const auto size = layout.compute_storage_requirement();

    REQUIRE( expected_size == size );
}


TEST_CASE("compute_storage_requirement in strided_layout should return 0 if there is a zero-sized axis", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        32, 0, 12, 6
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        1, 32, 512, 8192
    };
    const auto offset = 20;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    const auto size = layout.compute_storage_requirement();

    REQUIRE( size == 0 );
}

TEST_CASE( "apply_subscripts in strided_layout should implicitly fill the reminding axes", "[strided_layout]" )
{
    const std::vector<std::size_t> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::size_t offset = 16;
    const strided_layout layout(
        extents.data(),
        extents.size(),
        offset
    );
    const std::vector<dynamic_subscript> subscripts = 
    {
        0,
        0
    };

    const auto subscripted_layout = layout.apply_subscripts(xmipp4::make_span(subscripts));

    const std::vector<std::size_t> expected_extents = 
    {
        24,
        1,
        10,
        8
    };
    const std::vector<std::ptrdiff_t> expected_strides = 
    {
        80,
        80, 
        8, 
        1
    };

    const std::ptrdiff_t expected_offset = offset;

    REQUIRE( subscripted_layout.get_offset() == expected_offset );
    REQUIRE( subscripted_layout.get_rank() == expected_extents.size() );

    const auto count = subscripted_layout.get_rank();
    strided_axis axis;
    for (std::size_t i = 0; i < count; ++i)
    {
        subscripted_layout.get_axis(i, axis);
        REQUIRE( axis.get_extent() == expected_extents[i] );
        REQUIRE( axis.get_stride() == expected_strides[i] );
    }
}

TEST_CASE( "apply_subscripts in strided_layout should implicitly fill the reminding axes after an ellipsis", "[strided_layout]" )
{
    const std::vector<std::size_t> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::size_t offset = 16;
    const strided_layout layout(
        extents.data(),
        extents.size(),
        offset
    );
    const std::vector<dynamic_subscript> subscripts = 
    {
        ellipsis(),
        0,
        0
    };

    const auto subscripted_layout = layout.apply_subscripts(xmipp4::make_span(subscripts));

    const std::vector<std::size_t> expected_extents = 
    {
        120,
        56,
        24,
        1
    };
    const std::vector<std::ptrdiff_t> expected_strides = 
    {
        107520,
        1920,
        80,
        80
    };

    const std::ptrdiff_t expected_offset = offset;

    REQUIRE( subscripted_layout.get_offset() == expected_offset );
    REQUIRE( subscripted_layout.get_rank() == expected_extents.size() );

    const auto count = subscripted_layout.get_rank();
    strided_axis axis;
    for (std::size_t i = 0; i < count; ++i)
    {
        subscripted_layout.get_axis(i, axis);
        REQUIRE( axis.get_extent() == expected_extents[i] );
        REQUIRE( axis.get_stride() == expected_strides[i] );
    }
}


TEST_CASE( "apply_subscripts in strided_layout with a complex subscript should produce the expected result", "[strided_layout]" )
{
    const std::vector<std::size_t> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::size_t offset = 16;
    const strided_layout layout(
        extents.data(),
        extents.size(),
        offset
    );
    const std::vector<dynamic_subscript> subscripts = 
    {
        odd(), // Selects half of the elements in the axis with 120 elements
        new_axis(), // Inserts phantom axis between 120 and 56 sized axes
        ellipsis(), // Absorbs 56 and 24 sized axes.
        0, // Squeezes 1-sized axis
        even(), // Selects half of the elements in the axis with 10 elements
        new_axis(), // Inserts an axis between 10 and 8 sized axes
        new_axis(), // Inserts an axis between 10 and 8 sized axes
        6 // Selects the 7th element in the 8 element axis
    };

    const auto subscripted_layout = layout.apply_subscripts(xmipp4::make_span(subscripts));

    const std::vector<std::size_t> expected_extents = 
    {
        60,
        1,
        56,
        24,
        5,
        1,
        1
    };
    const std::vector<std::ptrdiff_t> expected_strides = 
    {
        215040,
        0,
        1920,
        80,
        16,
        0,
        0
    };

    const std::ptrdiff_t expected_offset = offset + 107520 + 6;

    REQUIRE( subscripted_layout.get_offset() == expected_offset );
    REQUIRE( subscripted_layout.get_rank() == expected_extents.size() );

    const auto count = subscripted_layout.get_rank();
    strided_axis axis;
    for (std::size_t i = 0; i < count; ++i)
    {
        subscripted_layout.get_axis(i, axis);
        REQUIRE( axis.get_extent() == expected_extents[i] );
        REQUIRE( axis.get_stride() == expected_strides[i] );
    }
}

TEST_CASE( "apply_subscripts in strided_layout with two ellipsis should throw", "[strided_layout]" )
{
    const std::vector<std::size_t> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::size_t offset = 16;
    const strided_layout layout(
        extents.data(),
        extents.size(),
        offset
    );
    const std::vector<dynamic_subscript> subscripts = 
    {
        ellipsis(),
        new_axis(),
        ellipsis()
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.apply_subscripts(xmipp4::make_span(subscripts)), "Two ellipsis tags were encountered when processing subscripts" );
}

TEST_CASE( "apply_subscripts in strided_layout with too many subscripts should throw", "[strided_layout]" )
{
    const std::vector<std::size_t> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::size_t offset = 16;
    const strided_layout layout(
        extents.data(),
        extents.size(),
        offset
    );
    const std::vector<dynamic_subscript> subscripts = 
    {
        6,
        2, 
        2,
        odd(),
        even(),
        all(),
        2
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.apply_subscripts(xmipp4::make_span(subscripts)), "Not all subscripts were processed" );
}

TEST_CASE( "apply_subscripts in strided_layout with too many subscripts and ellipsis should throw", "[strided_layout]" )
{
    const std::vector<std::size_t> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::size_t offset = 16;
    const strided_layout layout(
        extents.data(),
        extents.size(),
        offset
    );
    const std::vector<dynamic_subscript> subscripts = 
    {
        6,
        2, 
        2,
        ellipsis(),
        odd(),
        even(),
        all(),
        2
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.apply_subscripts(xmipp4::make_span(subscripts)), "Not all subscripts were processed" );
}

TEST_CASE( "apply_subscripts in strided_layout with out of bounds index should throw", "[strided_layout]" )
{
    const std::vector<std::size_t> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::size_t offset = 16;
    const strided_layout layout(
        extents.data(),
        extents.size(),
        offset
    );
    const std::vector<dynamic_subscript> subscripts = 
    {
        121
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::out_of_range );
}

TEST_CASE( "apply_subscripts in strided_layout with out of bounds index after ellipsis should throw", "[strided_layout]" )
{
    const std::vector<std::size_t> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::size_t offset = 16;
    const strided_layout layout(
        extents.data(),
        extents.size(),
        offset
    );
    const std::vector<dynamic_subscript> subscripts = 
    {
        ellipsis(),
        9
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::out_of_range );
}

TEST_CASE( "apply_subscripts in strided_layout with out of bounds slice should throw", "[strided_layout]" )
{
    const std::vector<std::size_t> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::size_t offset = 16;
    const strided_layout layout(
        extents.data(),
        extents.size(),
        offset
    );
    const std::vector<dynamic_subscript> subscripts = 
    {
        make_slice(121)
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::out_of_range );
}

TEST_CASE( "apply_subscripts in strided_layout with out of bounds slice after ellipsis should throw", "[strided_layout]" )
{
    const std::vector<std::size_t> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::size_t offset = 16;
    const strided_layout layout(
        extents.data(),
        extents.size(),
        offset
    );
    const std::vector<dynamic_subscript> subscripts = 
    {
        ellipsis(),
        make_slice(9)
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::out_of_range );
}

TEST_CASE("transpose in strided_layout should reverse the order of its axes", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    const auto transposed_layout = layout.transpose();

    REQUIRE( transposed_layout.get_rank() == layout.get_rank() );
    REQUIRE( transposed_layout.get_offset() == layout.get_offset() );

    const auto count = transposed_layout.get_rank();
    std::vector<std::size_t> read_extents(count);
    std::vector<std::ptrdiff_t> read_strides(count);
    transposed_layout.get_axes(read_extents.data(), read_strides.data(), count);

    REQUIRE( std::equal(extents.cbegin(), extents.cend(), read_extents.crbegin()) );
    REQUIRE( std::equal(strides.cbegin(), strides.cend(), read_strides.crbegin()) );
}

TEST_CASE("permute in strided_layout should correctly alter the order of the axes", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );
    const std::vector<std::size_t> permutation = 
    {
        0, 3, 2, 1
    };

    const auto permuted_layout = layout.permute(xmipp4::make_span(permutation));

    REQUIRE( permuted_layout.get_rank() == layout.get_rank() );
    REQUIRE( permuted_layout.get_offset() == layout.get_offset() );

    const auto count = permuted_layout.get_rank();
    strided_axis axis;
    for (std::size_t i = 0; i < count; ++i)
    {
        permuted_layout.get_axis(i, axis);
        const auto index = permutation[i];
        REQUIRE( axis.get_extent() == extents[index] );
        REQUIRE( axis.get_stride() == strides[index] );
    }
}

TEST_CASE("permute in strided_layout should throw with an invalid permutation", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );
    const std::vector<std::size_t> permutation = 
    {
        0, 3, 4, 1
    };

    REQUIRE_THROWS_AS( layout.permute(xmipp4::make_span(permutation)), std::invalid_argument );
}

TEST_CASE("swap_axes in strided_layout should swap the requested axes", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    const auto swapped_layout = layout.swap_axes(0, 2);

    REQUIRE( swapped_layout.get_rank() == layout.get_rank() );
    REQUIRE( swapped_layout.get_offset() == layout.get_offset() );

    const std::vector<std::size_t> equivalent_permutation = 
    {
        2, 1, 0, 3
    };
    const auto count = swapped_layout.get_rank();
    strided_axis axis;
    for (std::size_t i = 0; i < count; ++i)
    {
        swapped_layout.get_axis(i, axis);
        const auto index = equivalent_permutation[i];
        REQUIRE( axis.get_extent() == extents[index] );
        REQUIRE( axis.get_stride() == strides[index] );
    }
}

TEST_CASE("swap_axes in strided_layout should throw when one of the axes is out of bounds", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    REQUIRE_THROWS_AS( layout.swap_axes(5, 0), std::out_of_range );
    REQUIRE_THROWS_WITH( layout.swap_axes(5, 0), "axis1 exceeds bounds" );
    REQUIRE_THROWS_AS( layout.swap_axes(0, 5), std::out_of_range );
    REQUIRE_THROWS_WITH( layout.swap_axes(0, 5), "axis2 exceeds bounds" );
}

TEST_CASE("squeeze in strided_layout should remove all axes of extent 1", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 1, 563, 1234, 1, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 6, 63, 12434, 8, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    const auto squeezed_layout = layout.squeeze();

    REQUIRE( squeezed_layout.get_rank() == 4 );
    REQUIRE( squeezed_layout.get_offset() == layout.get_offset() );

    const std::vector<std::size_t> expected_extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> expected_strides = 
    {
        23, 63, 12434, 532
    };

    const auto count = squeezed_layout.get_rank();
    strided_axis axis;
    for (std::size_t i = 0; i < count; ++i)
    {
        squeezed_layout.get_axis(i, axis);
        REQUIRE( axis.get_extent() == expected_extents[i] );
        REQUIRE( axis.get_stride() == expected_strides[i] );
    }
}

TEST_CASE("broadcast_dry in strided_layout should fill in the left and promote axes with extent 1", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    std::vector<std::size_t> broadcast_extents =
    {
        1, 245
    };

    layout.broadcast_dry(broadcast_extents);
    REQUIRE( broadcast_extents == extents );
}
TEST_CASE("broadcast_dry in strided_layout should fill in the left and promote axes with extent 1, leaving trailing dimensions out", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245, 543, 453
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532, 343, 355
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    std::vector<std::size_t> broadcast_extents =
    {
        1, 245
    };

    const auto trailing_dimensions = 2;
    layout.broadcast_dry(broadcast_extents, trailing_dimensions);

    const std::vector<std::size_t> expected_extents = 
    {
        123, 563, 1234, 245
    };
    REQUIRE( broadcast_extents == expected_extents );
}

TEST_CASE("broadcast_dry in strided_layout should throw when the provided extents have more axes than the layout", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    std::vector<std::size_t> broadcast_extents =
    {
        7, 123, 563, 1234, 245
    };

    REQUIRE_THROWS_AS( layout.broadcast_dry(broadcast_extents), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.broadcast_dry(broadcast_extents), "Can not broadcast shape with 5 axes into a layout of 4 axes with 0 trailing dimensions" );
}

TEST_CASE("broadcast_dry in strided_layout should throw if an axis is not broadcastable", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    std::vector<std::size_t> broadcast_extents =
    {
        2
    };

    REQUIRE_THROWS_AS( layout.broadcast_dry(broadcast_extents), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.broadcast_dry(broadcast_extents), "Can not broadcast extent 2 into an axis of extent 245" );
}

TEST_CASE("broadcast_dry in strided_layout should throw if requesting too many trailing axes", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );

    std::vector<std::size_t> broadcast_extents =
    {
        2
    };

    REQUIRE_THROWS_AS( layout.broadcast_dry(broadcast_extents, 5), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.broadcast_dry(broadcast_extents, 5), "Requesting more trailing dimensions than axes in the layout" );
}

TEST_CASE("broadcast_to in strided_layout should fill in the left and promote axes with extent 1", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 1,  1, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        3, 23, 63, 12434
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );
    const std::vector<std::size_t> target_extents =
    {
        9, 7, 123, 5, 8, 245
    };

    const auto broadcasted_layout = layout.broadcast_to(xmipp4::make_span(target_extents));

    REQUIRE( broadcasted_layout.get_rank() == target_extents.size() );
    REQUIRE( broadcasted_layout.get_offset() == layout.get_offset() );

    const std::vector<std::size_t> expected_strides =
    {
        0, 0, 3, 0, 0, 12434
    };

    const auto count = broadcasted_layout.get_rank();
    strided_axis axis;
    for (std::size_t i = 0; i < count; ++i)
    {
        broadcasted_layout.get_axis(i, axis);
        REQUIRE( axis.get_extent() == target_extents[i] );
        REQUIRE( axis.get_stride() == expected_strides[i] );
    }
}

TEST_CASE("broadcast_to in strided_layout should fill in the left and promote axes with extent 1 while passing through trailing dimensions", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 1,  1, 245, 234, 123
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        3, 23, 63, 12434, 322, 342
    };
    const std::size_t offset = 7533;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );
    const std::vector<std::size_t> target_extents =
    {
        9, 7, 123, 5, 8, 245
    };

    const auto trailing_dimensions = 2;
    const auto broadcasted_layout = layout.broadcast_to(
        xmipp4::make_span(target_extents),
        trailing_dimensions
    );

    REQUIRE( broadcasted_layout.get_rank() == target_extents.size() + trailing_dimensions );
    REQUIRE( broadcasted_layout.get_offset() == layout.get_offset() );

    const std::vector<std::size_t> expected_extents =
    {
        9, 7, 123, 5, 8, 245, 234, 123
    };
    const std::vector<std::size_t> expected_strides =
    {
        0, 0, 3, 0, 0, 12434, 322, 342
    };

    const auto count = broadcasted_layout.get_rank();
    strided_axis axis;
    for (std::size_t i = 0; i < count; ++i)
    {
        broadcasted_layout.get_axis(i, axis);
        REQUIRE( axis.get_extent() == expected_extents[i] );
        REQUIRE( axis.get_stride() == expected_strides[i] );
    }
}

TEST_CASE("broadcast_to in strided_layout should throw when the provided extents have less axes than the layout", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 1,  1, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        3, 23, 63, 12434
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );
    const std::vector<std::size_t> target_extents =
    {
        8, 245
    };

    REQUIRE_THROWS_AS( layout.broadcast_to(xmipp4::make_span(target_extents), 1), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.broadcast_to(xmipp4::make_span(target_extents), 1), "Can not broadcast layout with 4 axes into a shape of 2 axes and 1 trailing dimensions." );
}

TEST_CASE("broadcast_to in strided_layout should throw if an axis is not broadcastable", "[strided_layout]")
{
    const std::vector<std::size_t> extents = 
    {
        123, 563, 1234, 245
    };
    const std::vector<std::ptrdiff_t> strides = 
    {
        23, 63, 12434, 532
    };
    const std::size_t offset = 47683;
    const strided_layout layout(
        extents.data(),
        strides.data(), 
        extents.size(),
        offset
    );
    const std::vector<std::size_t> target_extents =
    {
        2, 563, 1234, 245
    };

    REQUIRE_THROWS_AS( layout.broadcast_to(xmipp4::make_span(target_extents)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.broadcast_to(xmipp4::make_span(target_extents)), "Can not broadcast axis of extent 123 into an extent of 2." );
}
