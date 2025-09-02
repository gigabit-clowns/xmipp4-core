// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "strided_layout.hpp"
#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class layout_policy
{
public:
    virtual ~layout_policy() = default;

    virtual 
    strided_layout 
    layout_from_extents(span<const std::size_t> extents) const = 0;

};

const layout_policy & get_default_layout_policy() noexcept;

} // namespace multidimensional
} // namespace xmipp4
