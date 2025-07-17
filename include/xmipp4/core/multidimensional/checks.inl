// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "checks.hpp"

#include <algorithm>
#include <stdexcept>
#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

template <typename ForwardIt>
void check_axis_permutation(ForwardIt first, ForwardIt last, std::size_t count)
{
    // Based on:
    // https://en.cppreference.com/w/cpp/algorithm/is_permutation
    // The former function could be re-used, provided that something
    // like boost::counting_iterator is available.

    if (std::distance(first, last) != static_cast<std::ptrdiff_t>(count))
    {
        throw std::invalid_argument(
            "permutation's length does not match the required count"
        );
    }

    // Skip common prefix
    std::size_t i = 0;
    while (first != last && *first == i)
    {
        ++first;
        ++i;
    }

    // For the rest, check that it is a permutation
    while (i < count)
    {
        // Ensure the current value appears in the range.
        auto ite = std::find(first, last, i);
        if (ite == last)
        {
            std::ostringstream oss;
            oss << "value " << i << " is missing in the permutation";
            throw std::invalid_argument(oss.str());
        }

        ++i;
    }
}

} // namespace multidimensional
} // namespace xmipp4
