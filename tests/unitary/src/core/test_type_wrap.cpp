// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/type_wrap.hpp>

#include <type_traits>

using namespace xmipp4;

struct test_type {};

TEST_CASE( "type_tag should propagate the template argument to its type member", "[type_wrap]" ) 
{
	CHECK( std::is_same<type_tag<test_type>::type, test_type>::value );
}

TEST_CASE( "querying the size of a type_list should return the correct size", "[type_wrap]" ) 
{
	CHECK( type_list_size<type_list<float, int, float>>::value == 3);
	CHECK( type_list_size<type_list<int, test_type>>::value == 2);
	CHECK( type_list_size<type_list<test_type>>::value == 1);
	CHECK( type_list_size<type_list<>>::value == 0);
}

TEST_CASE( "indexing elements in a type_list should return the correct element", "[type_wrap]" ) 
{
	CHECK( std::is_same<type_list_element<0, type_list<float, int, float>>::type, float>::value );
	CHECK( std::is_same<type_list_element<0, type_list<test_type>>::type, test_type>::value );
	CHECK( std::is_same<type_list_element<1, type_list<test_type, int>>::type, int>::value );
}
