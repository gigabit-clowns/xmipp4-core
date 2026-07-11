// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/layout/joint_layout.hpp>

#include <core/layout/joint_layout_implementation.hpp>

#include <algorithm>
#include <stdexcept>

using namespace xmipp4;

TEST_CASE( "getting extents on an default constructed joint_layout should return empty ", "[joint_layout]" )
{
	joint_layout layout;
	const auto extents = layout.get_extents();
	REQUIRE( extents.empty() );
}

TEST_CASE( "getting extents on an default constructed joint_layout should have no operands ", "[joint_layout]" )
{
	joint_layout layout;
	REQUIRE( layout.get_number_of_operands() == 0 );
}

TEST_CASE( "getting strides on an default constructed joint_layout should throw ", "[joint_layout]" )
{
	joint_layout layout;
	REQUIRE_THROWS_AS( 
		layout.get_strides(0),
		std::out_of_range
	);
}

TEST_CASE( "getting the offset on an default constructed joint_layout should throw ", "[joint_layout]" )
{
	joint_layout layout;
	REQUIRE_THROWS_AS( 
		layout.get_offset(0),
		std::out_of_range
	);
};

TEST_CASE( "getting extents on an initialized joint_layout should return its extents ", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents =
		{ 20, 4, 16, 2 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	joint_layout layout(std::move(implementation));

	const auto result = layout.get_extents();
	REQUIRE( std::equal(extents.cbegin(), extents.cend(), result.begin(), result.end()) );
}

TEST_CASE( "getting the number of operands on an joint_layout should return its operand count", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 20, 4, 16, 2 };
	joint_layout_implementation::stride_vector_type strides = 
		{ 1, 20, 80, 1280 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides, 0UL);
	implementation->add_operand(strides, 0UL);
	joint_layout layout(std::move(implementation));

	REQUIRE( layout.get_number_of_operands() == 2 );
}

TEST_CASE( "getting the strides of an operand on an joint_layout should return its strides", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 20, 4, 16, 2 };
	joint_layout_implementation::stride_vector_type strides = 
		{ 1, 20, 80, 1280 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides, 0UL);
	joint_layout layout(std::move(implementation));

	const auto result = layout.get_strides(0);
	REQUIRE( std::equal(strides.cbegin(), strides.cend(), result.begin(), result.end()) );
}

TEST_CASE( "getting the offset of an operand in an joint_layout should return its offset", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 20, 4, 16, 2 };
	joint_layout_implementation::stride_vector_type strides = 
		{ 1, 20, 80, 1280 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides, 1234UL);
	joint_layout layout(std::move(implementation));

	REQUIRE( layout.get_offset(0) == 1234UL );
}

TEST_CASE( "getting the strides of an invalid operand on an joint_layout should throw", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 20, 4, 16, 2 };
	joint_layout_implementation::stride_vector_type strides = 
		{ 1, 20, 80, 1280 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides, 0UL);
	joint_layout layout(std::move(implementation));

	REQUIRE_THROWS_AS( 
		layout.get_strides(1),
		std::out_of_range
	);
}

TEST_CASE( "getting the offset of an invalid operand in an joint_layout should throw", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 20, 4, 16, 2 };
	joint_layout_implementation::stride_vector_type strides = 
		{ 1, 20, 80, 1280 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides, 1234UL);
	joint_layout layout(std::move(implementation));

	REQUIRE_THROWS_AS( 
		layout.get_offset(1),
		std::out_of_range
	);
}

TEST_CASE( "calling iter on a default constructed array access layout should return 0", "[joint_layout]" )
{
	joint_layout layout;
	joint_cursor ite;

	CHECK( layout.iter(ite) == 0 );
}

TEST_CASE( "calling iter on an array access layout with an axis of size zero should return 0", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 20, 0, 16, 2 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	joint_layout layout(std::move(implementation));
	joint_cursor ite;

	CHECK( layout.iter(ite) == 0 );
}

TEST_CASE( "calling iter on an array access layout should populate the iterator and return the inner-most extent", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 20, 4, 16, 2 };
	joint_layout_implementation::stride_vector_type strides = 
		{ 1, 20, 80, 1280 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides, 1234UL);
	implementation->add_operand(strides, 6532UL);
	implementation->add_operand(strides, 4UL);
	implementation->add_operand(strides, 8UL);
	implementation->add_operand(strides, 2UL);
	joint_layout layout(std::move(implementation));
	joint_cursor ite;

	CHECK( layout.iter(ite) == extents.front() );
	
	const auto indices = ite.get_indices();
	CHECK( indices.size() == extents.size() );
	for (std::size_t i = 0; i < indices.size(); ++i)
	{
		CHECK( indices[i] == 0 );
	}

	const auto offsets = ite.get_offsets();
	REQUIRE( offsets.size() == 5 );
	CHECK( offsets[0] == 1234UL );
	CHECK( offsets[1] == 6532UL );
	CHECK( offsets[2] == 4UL );
	CHECK( offsets[3] == 8UL );
	CHECK( offsets[4] == 2UL );
}

TEST_CASE( "calling next on a default constructed array access layout should return 0", "[joint_layout]" )
{
	joint_layout layout;
	joint_cursor ite;

	CHECK( layout.next(ite, 1) == 0 );
}

TEST_CASE( "calling next on an array access layout on a stepping basis should advance indices and offsets", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 8, 2, 3 };
	joint_layout_implementation::stride_vector_type strides1 = 
		{ 1, 20, 80 };
	joint_layout_implementation::stride_vector_type strides2 = 
		{ 1, 8, 18 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides1, 2048UL);
	implementation->add_operand(strides2, 1024UL);

	joint_layout layout(std::move(implementation));
	joint_cursor ite;

	REQUIRE( layout.iter(ite) == extents.front() );

	const auto offsets = ite.get_offsets();
	const auto indices = ite.get_indices();
	REQUIRE( offsets.size() == 2 );
	REQUIRE( indices.size() == 3 );

	for (std::size_t i = 0; i < extents[2]; ++i)
	{
		for (std::size_t j = 0; j < extents[1]; ++j)
		{
			for (std::size_t k = 0; k < extents[0]; ++k)
			{
				REQUIRE( indices[0] == k );
				REQUIRE( indices[1] == j );
				REQUIRE( indices[2] == i );
	
				CHECK( 2048 + k*strides1[0] + j*strides1[1] + i*strides1[2] == offsets[0] );
				CHECK( 1024 + k*strides2[0] + j*strides2[1] + i*strides2[2] == offsets[1] );

				std::size_t expected;
				if (k == extents[0]-1)
				{
					if (j == extents[1]-1 && i == extents[2]-1)
					{
						expected = 0;
					}
					else
					{
						expected = extents[0];
					}
				}
				else
				{
					expected = extents[0] - k - 1;
				}

				REQUIRE( layout.next(ite, 1) == expected );
			}
		}
	}

	CHECK( indices[0] == 0 );
	CHECK( indices[1] == 0 );
	CHECK( indices[2] == 0 );
	CHECK( offsets[0] == 2048 );
	CHECK( offsets[1] == 1024 );
}

TEST_CASE( "calling next on an array access layout on a block basis should advance indices and offsets", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 15, 2, 4 };
	joint_layout_implementation::stride_vector_type strides1 = 
		{ 1, 20, 80 };
	joint_layout_implementation::stride_vector_type strides2 = 
		{ 1, 15, 32 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides1, 2048UL);
	implementation->add_operand(strides2, 1024UL);

	joint_layout layout(std::move(implementation));
	joint_cursor ite;

	REQUIRE( layout.iter(ite) == 15 );

	const auto offsets = ite.get_offsets();
	const auto indices = ite.get_indices();
	REQUIRE( offsets.size() == 2 );
	REQUIRE( indices.size() == 3 );

	for (std::size_t i = 0; i < extents[2]; ++i)
	{
		for (std::size_t j = 0; j < extents[1]; ++j)
		{
			REQUIRE( indices[0] == 0 );
			REQUIRE( indices[1] == j );
			REQUIRE( indices[2] == i );
			CHECK( 2048 + j*strides1[1] + i*strides1[2] == offsets[0] );
			CHECK( 1024 + j*strides2[1] + i*strides2[2] == offsets[1] );

			REQUIRE( layout.next(ite, 8) == 7 );

			REQUIRE( indices[0] == 8 );
			REQUIRE( indices[1] == j );
			REQUIRE( indices[2] == i );
			CHECK( 2048 + 8*strides1[0] + j*strides1[1] + i*strides1[2] == offsets[0] );
			CHECK( 1024 + 8*strides2[0] + j*strides2[1] + i*strides2[2] == offsets[1] );

			std::size_t expected;
			if (j == extents[1]-1 && i == extents[2]-1)
			{
				expected = 0;
			}
			else
			{
				expected = 15;
			}

			REQUIRE( layout.next(ite, 7) == expected );
		}
	}

	CHECK( indices[0] == 0 );
	CHECK( indices[1] == 0 );
	CHECK( indices[2] == 0 );
	CHECK( offsets[0] == 2048 );
	CHECK( offsets[1] == 1024 );
}

TEST_CASE( "calling next on an array access layout on an first_dim should posterior indices and offsets", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 4, 15, 4 };
	joint_layout_implementation::stride_vector_type strides1 = 
		{ 1, 5, 80 };
	joint_layout_implementation::stride_vector_type strides2 = 
		{ 1, 4, 60 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides1, 2048UL);
	implementation->add_operand(strides2, 1024UL);

	joint_layout layout(std::move(implementation));
	joint_cursor ite;

	REQUIRE( layout.iter(ite, 1) == 15 );

	const auto offsets = ite.get_offsets();
	const auto indices = ite.get_indices();
	REQUIRE( offsets.size() == 2 );
	REQUIRE( indices.size() == 3 );

	const auto first_index = GENERATE(0, 2, 3);
	indices[0] = first_index; // Should not be modified.

	for (std::size_t i = 0; i < extents[2]; ++i)
	{
		REQUIRE( indices[0] == first_index );
		REQUIRE( indices[1] == 0 );
		REQUIRE( indices[2] == i );
		REQUIRE( 2048 + i*strides1[2] == offsets[0] );
		REQUIRE( 1024 + i*strides2[2] == offsets[1] );

		REQUIRE( layout.next(ite, 8, 1) == 7 );

		REQUIRE( indices[0] == first_index );
		REQUIRE( indices[1] == 8 );
		REQUIRE( indices[2] == i );
		REQUIRE( 2048 + 8*strides1[1] + i*strides1[2] == offsets[0] );
		REQUIRE( 1024 + 8*strides2[1] + i*strides2[2] == offsets[1] );

		std::size_t expected;
		if (i == extents[2]-1)
		{
			expected = 0;
		}
		else
		{
			expected = 15;
		}

		REQUIRE( layout.next(ite, 7, 1) == expected );
	}

	CHECK( indices[0] == first_index );
	CHECK( indices[1] == 0 );
	CHECK( indices[2] == 0 );
	CHECK( offsets[0] == 2048 );
	CHECK( offsets[1] == 1024 );
}

TEST_CASE( "calling next on an array access layout with first_dim and last_dim should update in-between indices and offsets", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 4, 15, 4 };
	joint_layout_implementation::stride_vector_type strides1 = 
		{ 1, 5, 80 };
	joint_layout_implementation::stride_vector_type strides2 = 
		{ 1, 4, 60 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides1, 2048UL);
	implementation->add_operand(strides2, 1024UL);

	joint_layout layout(std::move(implementation));
	joint_cursor ite;

	REQUIRE( layout.iter(ite, 1, 2) == 15 );

	const auto offsets = ite.get_offsets();
	const auto indices = ite.get_indices();
	REQUIRE( offsets.size() == 2 );
	REQUIRE( indices.size() == 3 );

	const auto first_index = GENERATE(0, 2, 3);
	const auto last_index = GENERATE(0, 2, 3);
	indices[0] = first_index; // Should not be modified.
	indices[2] = last_index; // Should not be modified.

	REQUIRE( indices[1] == 0 );
	REQUIRE( offsets[0] == 2048);
	REQUIRE( offsets[1] == 1024);

	REQUIRE( layout.next(ite, 8, 1, 2) == 7 );

	REQUIRE( indices[0] == first_index );
	REQUIRE( indices[1] == 8 );
	REQUIRE( indices[2] == last_index );
	REQUIRE( 2048 + 8*strides1[1] == offsets[0] );
	REQUIRE( 1024 + 8*strides2[1] == offsets[1] );

	REQUIRE( layout.next(ite, 7, 1, 2) == 0 );
}

TEST_CASE( "calling next on an array access layout with same first_dim and last_dim should prompt single iteration", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 4, 15, 4 };
	joint_layout_implementation::stride_vector_type strides1 = 
		{ 1, 5, 80 };
	joint_layout_implementation::stride_vector_type strides2 = 
		{ 1, 4, 60 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides1, 2048UL);
	implementation->add_operand(strides2, 1024UL);

	joint_layout layout(std::move(implementation));
	joint_cursor ite;

	REQUIRE( layout.iter(ite, 2, 2) == 1 );
	REQUIRE( layout.next(ite, 1, 2, 2) == 0 );
}

TEST_CASE( "calling next on an array access layout with zero step should not advance the iterator", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 4, 15, 4 };
	joint_layout_implementation::stride_vector_type strides = 
		{ 1, 5, 80 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides, 1024);

	joint_layout layout(std::move(implementation));
	joint_cursor ite;

	const auto dim = GENERATE(0, 1, 2);
	REQUIRE( layout.iter(ite, dim) == extents[dim] );

	const auto expected_ite = ite;
	const auto expected_offsets = expected_ite.get_offsets();
	const auto expected_indices = expected_ite.get_indices();
	for (std::size_t i = 0; i < 100; ++i)
	{
		REQUIRE( layout.next(ite, 0, dim) == extents[dim] );

		const auto offsets = ite.get_offsets();
		const auto indices = ite.get_indices();
		REQUIRE( std::equal(expected_offsets.begin(), expected_offsets.end(), offsets.begin(), offsets.end()) );
		REQUIRE( std::equal(expected_indices.begin(), expected_indices.end(), indices.begin(), indices.end()) );
	}

}

TEST_CASE( "calling iter on a joint_layout with dim equal to rank should return one", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 4, 15, 4 };
	joint_layout_implementation::stride_vector_type strides = 
		{ 1, 5, 80 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides, 1024);

	joint_layout layout(std::move(implementation));

	joint_cursor ite;
	REQUIRE( layout.iter(ite, 3) == 1 );
}

TEST_CASE( "calling next on a joint_layout with dim equal to rank should return zero", "[joint_layout]" )
{
	joint_layout_implementation::extent_vector_type extents = 
		{ 4, 15, 4 };
	joint_layout_implementation::stride_vector_type strides = 
		{ 1, 5, 80 };
	auto implementation = 
		std::make_unique<joint_layout_implementation>(extents);
	implementation->add_operand(strides, 1024);

	joint_layout layout(std::move(implementation));

	joint_cursor ite;
	REQUIRE( layout.iter(ite, 3) == 1 );
	REQUIRE( layout.next(ite, 1, 3) == 0 );
}

