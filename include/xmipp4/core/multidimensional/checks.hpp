// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Check that the permutation is valid for a given layout.
 * 
 * A permutation is valid if it is a permutation of the [0, count) range.
 * 
 * @tparam ForwardIt Forward iterator to std::size_t-s.
 * @param first Iterator to the first element in the range.
 * @param last Iterator to the past-the-end element in the range.
 * @param count Number of axes in the layout.
 * @throws std::invalid_argument If the permutation is not valid.
 */
template <typename ForwardIt>
void check_axis_permutation(ForwardIt first, ForwardIt last, std::size_t count);

} // namespace multidimensional
} // namespace xmipp4

#include "checks.inl"
