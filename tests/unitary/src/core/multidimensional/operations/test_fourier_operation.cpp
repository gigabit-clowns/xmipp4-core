// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operations/fourier_operation.hpp>

#include <cmath>

using namespace xmipp4;
using namespace xmipp4::multidimensional;
using Catch::Approx;

TEST_CASE(
    "get_forward_fft_normalization_factor returns 1 for none convention",
    "[fourier_operation]"
)
{
    const auto n = GENERATE(1UL, 4UL, 8UL, 16UL);
    CHECK( get_forward_fft_normalization_factor(fft_normalization::none, n)
           == Approx(1.0) );
}

TEST_CASE(
    "get_forward_fft_normalization_factor returns 1 for backward convention",
    "[fourier_operation]"
)
{
    const auto n = GENERATE(1UL, 4UL, 8UL, 16UL);
    CHECK( get_forward_fft_normalization_factor(fft_normalization::backward, n)
           == Approx(1.0) );
}

TEST_CASE(
    "get_forward_fft_normalization_factor returns 1/sqrt(n) for ortho convention",
    "[fourier_operation]"
)
{
    const auto n = GENERATE(1UL, 4UL, 9UL, 16UL);
    CHECK( get_forward_fft_normalization_factor(fft_normalization::ortho, n)
           == Approx(1.0 / std::sqrt(static_cast<double>(n))) );
}

TEST_CASE(
    "get_forward_fft_normalization_factor returns 1/n for forward convention",
    "[fourier_operation]"
)
{
    const auto n = GENERATE(1UL, 4UL, 8UL, 16UL);
    CHECK( get_forward_fft_normalization_factor(fft_normalization::forward, n)
           == Approx(1.0 / static_cast<double>(n)) );
}

TEST_CASE(
    "get_backward_fft_normalization_factor returns 1 for none convention",
    "[fourier_operation]"
)
{
    const auto n = GENERATE(1UL, 4UL, 8UL, 16UL);
    CHECK( get_backward_fft_normalization_factor(fft_normalization::none, n)
           == Approx(1.0) );
}

TEST_CASE(
    "get_backward_fft_normalization_factor returns 1 for forward convention",
    "[fourier_operation]"
)
{
    const auto n = GENERATE(1UL, 4UL, 8UL, 16UL);
    CHECK( get_backward_fft_normalization_factor(fft_normalization::forward, n)
           == Approx(1.0) );
}

TEST_CASE(
    "get_backward_fft_normalization_factor returns 1/sqrt(n) for ortho convention",
    "[fourier_operation]"
)
{
    const auto n = GENERATE(1UL, 4UL, 9UL, 16UL);
    CHECK( get_backward_fft_normalization_factor(fft_normalization::ortho, n)
           == Approx(1.0 / std::sqrt(static_cast<double>(n))) );
}

TEST_CASE(
    "get_backward_fft_normalization_factor returns 1/n for backward convention",
    "[fourier_operation]"
)
{
    const auto n = GENERATE(1UL, 4UL, 8UL, 16UL);
    CHECK( get_backward_fft_normalization_factor(fft_normalization::backward, n)
           == Approx(1.0 / static_cast<double>(n)) );
}

TEST_CASE(
    "get_fft_normalization_factor matches get_forward_fft_normalization_factor "
    "for forward direction",
    "[fourier_operation]"
)
{
    const auto [conv, n] = GENERATE(table<fft_normalization, std::size_t>({
        { fft_normalization::none,     8UL },
        { fft_normalization::ortho,    4UL },
        { fft_normalization::forward,  8UL },
        { fft_normalization::backward, 8UL },
    }));

    CHECK(
        get_fft_normalization_factor(conv, fft_direction::forward, n)
        == Approx(get_forward_fft_normalization_factor(conv, n))
    );
}

TEST_CASE(
    "get_fft_normalization_factor matches get_backward_fft_normalization_factor "
    "for backward direction",
    "[fourier_operation]"
)
{
    const auto [conv, n] = GENERATE(table<fft_normalization, std::size_t>({
        { fft_normalization::none,     8UL },
        { fft_normalization::ortho,    4UL },
        { fft_normalization::forward,  8UL },
        { fft_normalization::backward, 8UL },
    }));

    CHECK(
        get_fft_normalization_factor(conv, fft_direction::backward, n)
        == Approx(get_backward_fft_normalization_factor(conv, n))
    );
}

TEST_CASE(
    "get_fft_normalization_factor: ortho forward and backward factors "
    "are equal and their product is 1/n",
    "[fourier_operation]"
)
{
    const auto n = GENERATE(1UL, 4UL, 9UL, 16UL);
    const double fwd = get_fft_normalization_factor(
        fft_normalization::ortho, fft_direction::forward, n
    );
    const double bwd = get_fft_normalization_factor(
        fft_normalization::ortho, fft_direction::backward, n
    );
    CHECK( fwd == Approx(bwd) );
    CHECK( fwd * bwd == Approx(1.0 / static_cast<double>(n)) );
}
