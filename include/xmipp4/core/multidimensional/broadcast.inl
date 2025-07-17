// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "broadcast.hpp"

#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename... Broadcastables>
inline
std::tuple<Broadcastables...> broadcast(std::vector<std::size_t> &extents,
                                       const Broadcastables&... items )
{
    (items.broadcast_extents_to_layout(extents), ...); // TODO avoid using fold expressions
    return std::tuple<Broadcastables...>(
        items.broadcast_layout_to_extents(xmipp4::make_span(extents))...
    );
}


} // namespace multidimensional
} // namespace xmipp4
