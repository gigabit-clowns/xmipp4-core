#pragma once

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
 * @file checks.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of checks.hpp
 * @date 2025-02-06
 * 
 */

#include "checks.hpp"

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

    if (std::distance(first, last) != count)
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
    for (; i < count; ++i)
    {
        // Ensure the current value appears in the range.
        ForwardIt ite = std::find(first, last, i);
        if (ite == last)
        {
            std::ostringstream oss;
            oss << "value " << i << " is missing in the permutation";
            throw std::invalid_argument(oss.str());
        }

        // Ensure the current value appears strictly once in the range.
        ite = std::find(ite, last, i);
        if (ite != last)
        {
            std::ostringstream oss;
            oss << "value " << i << " is duplicated in the permutation";
            throw std::invalid_argument(oss.str());
        }
    }
}

} // namespace multidimensional
} // namespace xmipp4
