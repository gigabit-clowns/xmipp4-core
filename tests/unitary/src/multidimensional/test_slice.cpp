// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/slice.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <type_traits>
#include <sstream>

using namespace xmipp4::multidimensional;


TEST_CASE( "cross-constructing an slice should propagate values", "[slice]" )
{
    SECTION( "with integers" )
    {
        XMIPP4_CONST_CONSTEXPR auto a = make_slice(1, 3, 2);
        XMIPP4_CONST_CONSTEXPR auto b = slice<std::size_t, std::size_t, std::size_t>(a);

        REQUIRE( b.get_start() == 1 );
        REQUIRE( b.get_step() == 2 );
        REQUIRE( b.get_count() == 3 );
    }

    SECTION( "with integral constant" )
    {
        XMIPP4_CONST_CONSTEXPR auto a = make_slice(
            std::integral_constant<int, 1>(),
            std::integral_constant<int, 3>(),
            std::integral_constant<int, 2>()
        );
        XMIPP4_CONST_CONSTEXPR auto b = slice<std::size_t, std::size_t, std::size_t>(a);

        REQUIRE( b.get_start() == 1 );
        REQUIRE( b.get_step() == 2 );
        REQUIRE( b.get_count() == 3 );
    }

    SECTION( "with tags" )
    {
        XMIPP4_CONST_CONSTEXPR auto a = make_slice(std::size_t(8));
        XMIPP4_CONST_CONSTEXPR auto b = slice<int, int, int>(a);

        REQUIRE( b.get_start() == 0 );
        REQUIRE( b.get_step() == 1 );
        REQUIRE( b.get_count() == 8 );
    }

    SECTION( "integer promotion preserves end value")
    {
        const auto s0 = make_slice(1, end(), 2);
        const slice<int, std::uint8_t, int> s1(s0);
        const slice<int, std::int64_t, int> s2(s1);
        const slice<int, std::uint32_t, int> s3(s2);
        const slice<int, int, int> s4(s3);

        REQUIRE( s1.get_count() == end() );
        REQUIRE( s2.get_count() == end() );
        REQUIRE( s3.get_count() == end() );
        REQUIRE( s4.get_count() == end() );
    }
}

TEST_CASE("begin_tag should be equal to itself", "[slice]")
{
    REQUIRE(begin() == begin());
}

TEST_CASE("begin_tag should not be unequal to itself", "[slice]")
{
    REQUIRE(!(begin() != begin()));
}

TEST_CASE("begin_tag should be equal to 0", "[slice]")
{
    REQUIRE(begin() == 0);
    REQUIRE(0 == begin());
    REQUIRE(!(begin() != 0));
    REQUIRE(!(0 != begin()));
}

TEST_CASE("begin_tag should be unequal to non-zero integers", "[slice]")
{
    REQUIRE(begin() != 1);
    REQUIRE(1 != begin());
    REQUIRE(begin() != -10);
    REQUIRE(-10 != begin());
    REQUIRE(!(begin() == -10));
    REQUIRE(!(-10 == begin()));
}

TEST_CASE("end_tag should be equal to itself", "[slice]")
{
    REQUIRE(end() == end());
}

TEST_CASE("end_tag should not be unequal to itself", "[slice]")
{
    REQUIRE(!(end() != end()));
}

TEST_CASE("end_tag should be equal to std::numeric_limits::max()", "[slice]")
{
    REQUIRE(end() == std::numeric_limits<std::size_t>::max());
    REQUIRE(std::numeric_limits<std::size_t>::max() == end());
    REQUIRE(end() == std::numeric_limits<int>::max());
    REQUIRE(std::numeric_limits<int>::max() == end());
    REQUIRE(!(end() != std::numeric_limits<int>::max()));
    REQUIRE(!(std::numeric_limits<int>::max() != end()));
}

TEST_CASE("end_tag should be equal to integers other than std::numeric_limits::max()", "[slice]")
{
    REQUIRE(end() != 1);
    REQUIRE(1 != end());
    REQUIRE(end() != -10);
    REQUIRE(-10 != end());
    REQUIRE(!(end() == -10));
    REQUIRE(!(-10 == end()));
}

TEST_CASE("propagate_end should preserve normal values", "[slice]")
{
    REQUIRE(propagate_end<std::size_t>(5) == 5);
    REQUIRE(propagate_end<int>(10) == 10);
    REQUIRE(propagate_end<int>(-10) == -10);
    REQUIRE(propagate_end<int>(std::integral_constant<std::size_t, 9>()) == 9);
}

TEST_CASE("propagate_end should adjust end values", "[slice]")
{
    REQUIRE(propagate_end<std::size_t>(end()) == end());
    REQUIRE(propagate_end<std::int32_t>(std::numeric_limits<std::int64_t>::max()) == end());
    REQUIRE(propagate_end<std::int64_t>(std::numeric_limits<std::int32_t>::max()) == end());
    REQUIRE(propagate_end<std::uint32_t>(std::numeric_limits<std::uint64_t>::max()) == end());
    REQUIRE(propagate_end<std::uint64_t>(std::numeric_limits<std::uint32_t>::max()) == end());
}

TEST_CASE("adjacent_tag should be equal to itself", "[slice]")
{
    REQUIRE(adjacent() == adjacent());
}

TEST_CASE("adjacent_tag should not be unequal to itself", "[slice]")
{
    REQUIRE(!(adjacent() != adjacent()));
}

TEST_CASE("adjacent_tag should be equal to 1", "[slice]")
{
    REQUIRE(adjacent() == 1);
    REQUIRE(1 == adjacent());
    REQUIRE(!(adjacent() != 1));
    REQUIRE(!(1 != adjacent()));
}

TEST_CASE("adjacent_tag should be unequal to integers other than 1", "[slice]")
{
    REQUIRE(adjacent() != 2);
    REQUIRE(2 != adjacent());
    REQUIRE(adjacent() != -10);
    REQUIRE(-10 != adjacent());
    REQUIRE(!(adjacent() == 2));
    REQUIRE(!(2 == adjacent()));
}

TEST_CASE("reversed_tag should be equal to itself", "[slice]")
{
    REQUIRE(reversed() == reversed());
}

TEST_CASE("reversed_tag should not be unequal to itself", "[slice]")
{
    REQUIRE(!(reversed() != reversed()));
}

TEST_CASE("reversed_tag should be equal to -1", "[slice]")
{
    REQUIRE(reversed() == -1);
    REQUIRE(-1 == reversed());
    REQUIRE(!(reversed() != -1));
    REQUIRE(!(-1 != reversed()));
}

TEST_CASE("reversed_tag should be unequal to integers other than 1", "[slice]")
{
    REQUIRE(reversed() != 1);
    REQUIRE(1 != reversed());
    REQUIRE(reversed() != -10);
    REQUIRE(-10 != reversed());
    REQUIRE(!(reversed() == 1));
    REQUIRE(!(1 == reversed()));
}

TEST_CASE( "make_slice with count argument should set count and leave the rest defaulted", "[slice]" ) 
{
    SECTION( "with int" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(8);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const int>::value, 
            "count must be an int"
        );
        
        REQUIRE(count == 8);
    }

    SECTION( "with std::size_t" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(std::size_t(4));
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const std::size_t>::value, 
            "count must be a std::size_t"
        );
        
        REQUIRE(count == 4);
    }

    SECTION( "with end_tag" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(end());
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const end_tag>::value, 
            "count must be an end_tag"
        );
    }

    SECTION( "with integral constant" )
    {
        XMIPP4_CONST_CONSTEXPR auto five = std::integral_constant<int, 5>();
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(five);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const std::integral_constant<int, 5>>::value, 
            "count must be an std::integral_constant<int, 5>"
        );
    }
}

TEST_CASE( "make_slice with start and count arguments should set their values and leave the rest defaulted", "[slice]" ) 
{
    SECTION( "with int" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(1, 20);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const int>::value, 
            "Start must be an int"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const int>::value, 
            "count must be an int"
        );
        
        REQUIRE(start == 1);
        REQUIRE(count == 20);
    }

    SECTION( "with begin and end" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(begin(), end());
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const end_tag>::value, 
            "count must be an end_tag"
        );
    }

    SECTION( "with integral constant" )
    {
        XMIPP4_CONST_CONSTEXPR auto one = std::integral_constant<std::size_t, 1>();
        XMIPP4_CONST_CONSTEXPR auto five = std::integral_constant<int, 5>();
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(one, five);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const std::integral_constant<std::size_t, 1>>::value, 
            "Start must be a std::integral_constant<std::size_t, 1>"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const std::integral_constant<int, 5>>::value, 
            "count must be an std::integral_constant<int, 5>"
        );
    }
}

TEST_CASE( "make_slice with start stop and step arguments should set their values", "[slice]" ) 
{
    SECTION( "with int" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(1, 20, 2);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const int>::value, 
            "Start must be an int"
        );
        static_assert(
            std::is_same<decltype(step), const int>::value, 
            "Stride must be an int"
        );
        static_assert(
            std::is_same<decltype(count), const int>::value, 
            "count must be an int"
        );
        
        REQUIRE(start == 1);
        REQUIRE(step == 2);
        REQUIRE(count == 20);
    }

    SECTION( "with begin, adjacent and end" )
    {
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(begin(), end(), adjacent());
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const begin_tag>::value, 
            "Start must be a begin_tag"
        );
        static_assert(
            std::is_same<decltype(step), const adjacent_tag>::value, 
            "Stride must be an adjacent_tag"
        );
        static_assert(
            std::is_same<decltype(count), const end_tag>::value, 
            "count must be an end_tag"
        );
    }

    SECTION( "with integral constant" )
    {
        XMIPP4_CONST_CONSTEXPR auto one = std::integral_constant<std::size_t, 1>();
        XMIPP4_CONST_CONSTEXPR auto two = std::integral_constant<std::size_t, 2>();
        XMIPP4_CONST_CONSTEXPR auto five = std::integral_constant<int, 5>();
        XMIPP4_CONST_CONSTEXPR auto s = make_slice(one, five, two);
        XMIPP4_CONST_CONSTEXPR auto start = s.get_start();
        XMIPP4_CONST_CONSTEXPR auto step = s.get_step();
        XMIPP4_CONST_CONSTEXPR auto count = s.get_count();

        static_assert(
            std::is_same<decltype(start), const std::integral_constant<std::size_t, 1>>::value, 
            "Start must be a std::integral_constant<std::size_t, 1>"
        );
        static_assert(
            std::is_same<decltype(step), const std::integral_constant<std::size_t, 2>>::value, 
            "Stride must be an std::integral_constant<std::size_t, 2>"
        );
        static_assert(
            std::is_same<decltype(count), const std::integral_constant<int, 5>>::value, 
            "count must be an std::integral_constant<int, 5>"
        );
    }
}

TEST_CASE( "output slice to a std::ostream should produce the expected strings", "[slice]" )
{
    std::stringstream stream;

    SECTION( "with int-s" )
    {
        XMIPP4_CONST_CONSTEXPR auto x = make_slice(1, 3, 2);
        stream << x;

        REQUIRE( stream.str() == "slice(1, 3, 2)" );
    }

    SECTION( "with tags" )
    {
        XMIPP4_CONST_CONSTEXPR auto x = make_slice(end());
        stream << x;

        REQUIRE( stream.str() == "slice(begin, end, adjacent)" );
    }
}

TEST_CASE( "sanitize_slice should throw with a stride of zero" )
{
    const dynamic_slice input(2, 5, 0);
    const std::size_t extent = GENERATE(0, 1, 10, 20);

    REQUIRE_THROWS_AS( sanitize_slice(input, extent), std::invalid_argument );
    REQUIRE_THROWS_WITH( sanitize_slice(input, extent), "Slice step cannot be zero.");
}

TEST_CASE( "sanitize_slice should pass correct positive start values" )
{
    const auto extent = 16;
    const std::ptrdiff_t count = 1;
    const std::ptrdiff_t step = GENERATE(1, -1, 10, -10);
    const std::ptrdiff_t start = GENERATE(0, 8, 15);

    dynamic_slice input(start, count, step);
    const auto output = sanitize_slice(input, extent);
    REQUIRE(output.get_start() == static_cast<std::size_t>(start));
    REQUIRE(output.get_count() == count);
    REQUIRE(output.get_step() == step);
}

TEST_CASE( "sanitize_slice should replace correct negative start values" )
{
    const auto extent = 16;
    const std::ptrdiff_t count = 1;
    const std::ptrdiff_t step = GENERATE(1, -1, 10, -10);
    std::ptrdiff_t start;
    std::size_t expected;
    std::tie(start, expected) = GENERATE(
        table<std::ptrdiff_t, std::size_t>({
            {-1, 15},
            {-4, 12},
            {-16, 0}
        })
    );
    
    dynamic_slice input(start, count, step);
    const auto output = sanitize_slice(input, extent);
    REQUIRE(output.get_start() == expected);
    REQUIRE(output.get_count() == count);
    REQUIRE(output.get_step() == step);
}

TEST_CASE( "sanitize_slice should throw with out of bounds start values" )
{
    const auto extent = 16;
    const std::ptrdiff_t count = 1;
    const std::ptrdiff_t step = GENERATE(1, -1, 10, -10);
    std::ptrdiff_t start;
    std::string expected_error_msg;
    std::tie(start, expected_error_msg) = GENERATE(
        table<std::ptrdiff_t, std::string>({
            {16, "Non-empty slice's start index 16 is out of bounds for extent 16." },
            {25, "Non-empty slice's start index 25 is out of bounds for extent 16." },
            {99, "Non-empty slice's start index 99 is out of bounds for extent 16." },
            {-17, "Non-empty slice's start index -17 is out of bounds for extent 16." },
            {-50, "Non-empty slice's start index -50 is out of bounds for extent 16." },
        })
    );
    
    dynamic_slice input(start, count, step);
    REQUIRE_THROWS_AS( sanitize_slice(input, extent), std::out_of_range );
    REQUIRE_THROWS_WITH( sanitize_slice(input, extent), expected_error_msg );
}

TEST_CASE( "sanitize_slice should complete end values" )
{
    const auto extent = 16;
    const std::ptrdiff_t count = end();
    std::ptrdiff_t start;
    std::ptrdiff_t step;
    std::size_t expected_count;
    std::tie(start, step, expected_count) = GENERATE(
        table<std::ptrdiff_t, std::ptrdiff_t, std::size_t>({
            { 0, 1, 16 },
            { 1, 1, 15 },
            { 4, 1, 12 },
            { 15, 1, 1 },
            { 0, 4, 4 },
            { 3, 4, 4 },
            { 4, 4, 3 },
            { 15, 4, 1 },
            { 15, -1, 16 },
            { 14, -1, 15 },
            { 12, -1, 13 },
            { 0, -1, 1 },
            { 15, -4, 4 },
            { 12, -4, 4 },
            { 11, -4, 3 },
            { 0, -4, 1 },
        })
    );

    const dynamic_slice input(start, count, step);
    const auto output = sanitize_slice(input, extent);
    REQUIRE(output.get_start() == start);
    REQUIRE(output.get_count() == expected_count);
    REQUIRE(output.get_step() == step);
}


TEST_CASE( "sanitize_slice should throw with an out of bounds slice count value" )
{
    const auto extent = 16;
    std::ptrdiff_t start;
    std::size_t count;
    std::ptrdiff_t step;
    std::string expected_error_msg;
    std::tie(start, count, step, expected_error_msg) = GENERATE(
        table<std::ptrdiff_t, std::size_t, std::ptrdiff_t, std::string>({
            {0, 17, 1, "Slice count 17 start index 0 and step 1 overflows extent 16" },
            {2, 15, 1, "Slice count 15 start index 2 and step 1 overflows extent 16" },
            {0, 5, 4, "Slice count 5 start index 0 and step 4 overflows extent 16"},
            {6, 3, 5, "Slice count 3 start index 6 and step 5 overflows extent 16"},
            {15, 17, -1, "Reversed slice with count 17 start index 15 and step -1 underflows 0"},
            {10, 12, -1, "Reversed slice with count 12 start index 10 and step -1 underflows 0"},
            {4, 3, -4, "Reversed slice with count 3 start index 4 and step -4 underflows 0"},
        })
    );
    
    dynamic_slice input(start, count, step);
    REQUIRE_THROWS_AS( sanitize_slice(input, extent), std::out_of_range );
    REQUIRE_THROWS_WITH( sanitize_slice(input, extent), expected_error_msg );
}
