// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/layout/access_iterator.hpp>

using namespace xmipp4;

TEST_CASE( "default constructed array iterator should be empty", "[access_iterator]" )
{
	access_iterator iterator;
	CHECK( iterator.get_indices().data() == nullptr );
	CHECK( iterator.get_indices().size() == 0UL );
	CHECK( iterator.get_offsets().data() == nullptr );
	CHECK( iterator.get_offsets().size() == 0UL );
}

TEST_CASE( "array iterator should be initialized at zero position with base offsets", "[access_iterator]" )
{
	std::vector<std::ptrdiff_t> offsets = { 2345, 1234, 983, 1245 };
	const std::size_t n_dim = 12; 

	access_iterator iterator(n_dim, offsets);
	
	SECTION("const")
	{
		const auto indices = 
			static_cast<const access_iterator&>(iterator).get_indices();
	
		REQUIRE( indices.size() == n_dim );
		for (std::size_t i = 0; i < indices.size(); ++i)
		{
			CHECK(indices[i] == 0);
		}

		const auto offsets2 = 
			static_cast<const access_iterator&>(iterator).get_offsets();

		REQUIRE( offsets2.size() == offsets.size() );
		for (std::size_t i = 0; i < offsets2.size(); ++i)
		{
			CHECK(offsets2[i] == offsets[i]);
		}
	}
	SECTION("non-const")
	{
		const auto indices = iterator.get_indices();
	
		REQUIRE( indices.size() == n_dim );
		for (std::size_t i = 0; i < indices.size(); ++i)
		{
			CHECK(indices[i] == 0);
		}

		const auto offsets2 = iterator.get_offsets();

		REQUIRE( offsets2.size() == offsets.size() );
		for (std::size_t i = 0; i < offsets2.size(); ++i)
		{
			CHECK(offsets2[i] == offsets[i]);
		}
	}
}

TEST_CASE( "array iterator should not reallocate data", "[access_iterator]" )
{
	std::vector<std::ptrdiff_t> offsets = { 2345, 1234, 983, 1245 };
	const std::size_t n_dim = 12; 

	access_iterator iterator(n_dim, offsets);
	
	auto *offsets_ptr = iterator.get_offsets().data();
	auto *indices_ptr = iterator.get_indices().data();

	for (std::size_t i = 0; i < 16; ++i)
	{
		CHECK( offsets_ptr == iterator.get_offsets().data());
		CHECK( indices_ptr == iterator.get_indices().data());
	}
}
