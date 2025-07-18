// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/index.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>


using namespace xmipp4::multidimensional;



TEST_CASE("sanitize_index with a in-bounds positive index should return the same value", "[index]")
{
    const std::ptrdiff_t extent = 16;
    for (std::ptrdiff_t i = 0; i < extent; ++i)
    {
        const auto sanitized = sanitize_index(i, extent);
        REQUIRE( sanitized == i );
    }
}

TEST_CASE("sanitize_index with a in-bounds negative index should return the poisitivized value", "[index]")
{
    const std::ptrdiff_t extent = 16;
    for (std::ptrdiff_t i = 0; i < extent; ++i)
    {
        const auto index = -(i+1);
        const auto sanitized = sanitize_index(index, extent);
        REQUIRE( sanitized == (extent+index) ); // TODO parametrize
    }
}

TEST_CASE("sanitize_index with an out of bounds positive index should throw", "[index]")
{
    const std::ptrdiff_t extent = 16;
    const std::ptrdiff_t last = 32;
    for (std::ptrdiff_t i = extent; i < last; ++i)
    {
        std::ostringstream oss;
        oss << "Index " << i // TODO parametrize
            << " is out of bounds for extent " << extent;
        const auto err_msg = oss.str();

        REQUIRE_THROWS_AS( sanitize_index(i, extent), std::out_of_range );
        REQUIRE_THROWS_WITH( sanitize_index(i, extent), err_msg);
    }
}

TEST_CASE("sanitize_index with an out of bounds negative index should throw", "[index]")
{
    const std::ptrdiff_t extent = 16;
    const std::ptrdiff_t last = 32;
    for (std::ptrdiff_t i = extent; i < last; ++i)
    {
        const auto index = -(i+1);

        std::ostringstream oss; // TODO parametrize
        oss << "Reverse index " << index
            << " is out of bounds for extent " << extent;
        const auto err_msg = oss.str();

        REQUIRE_THROWS_AS( sanitize_index(index, extent), std::out_of_range );
        REQUIRE_THROWS_WITH( sanitize_index(index, extent), err_msg);
    }
}
