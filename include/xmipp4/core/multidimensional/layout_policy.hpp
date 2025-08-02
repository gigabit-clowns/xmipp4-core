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
    virtual 
    strided_layout 
    layout_from_extents(span<const std::size_t> extents) const = 0;

};

} // namespace multidimensional
} // namespace xmipp4
