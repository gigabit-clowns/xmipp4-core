// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <trompeloeil.hpp>

#include <xmipp4/core/span.hpp>

#include <vector>

namespace xmipp4
{
namespace multidimensional
{

class mock_broadcastable
{
public:
    static constexpr bool trompeloeil_movable_mock = true;

    MAKE_MOCK1(broadcast_extents_to_layout, void(std::vector<std::size_t>&), const);
    MAKE_MOCK1(broadcast_layout_to_extents, mock_broadcastable(span<const std::size_t>), const);

};

} // namespace multidimensional
} // namespace xmipp4
