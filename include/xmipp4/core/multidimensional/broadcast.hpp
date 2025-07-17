// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <vector>
#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Function to broadcast multiple layouts/arrays into a common
 * shape.
 * 
 * @tparam Broadcastables Broadastable types. These types must implement
 * broadcast_extents_to_layout and broadcast_layout_to_extents methods.
 * @param extents Input/output extents. The contents of extents participate
 * in the broadcasting operation. Leave it empty to avoid this. The resulting
 * consensus shape will be stored in this variable.
 * @param items Items to be broadcasted.
 * @return std::tuple<Broadcastables...> Broadcasted items. When successful,
 * all of the returned items will have the same shape as the values
 * written in extents.
 */
template <typename... Broadcastables>
std::tuple<Broadcastables...> broadcast(std::vector<std::size_t> &extents,
                                        const Broadcastables&... items );

} // namespace multidimensional
} // namespace xmipp4

#include "broadcast.inl"
