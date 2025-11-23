// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout;

/**
 * @brief Perform a broadcast operation between two sets of extents.
 * 
 * This operation pads the smallest set of extents with ones at the beginning. 
 * Then, both sets are iterated jointly. If both elements are equal, none of
 * them is modified. If unequal but one of them is 1, it is replaced by the
 * other. Otherwise an exception is thrown.
 * 
 * @param extents1 The first set of extents.
 * @param extents2 The second set of extents.
 * @throws std::broadcast_error when one of of the extents cannot be broadcasted.
 */
XMIPP4_CORE_API
void broadcast_extents(
	std::vector<std::size_t> &extents1, 
	std::vector<std::size_t> &extents2
);  

} // namespace multidimensional
} // namespace xmipp4
