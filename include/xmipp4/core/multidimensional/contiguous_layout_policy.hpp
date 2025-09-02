// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "layout_policy.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class contiguous_layout_policy final
    : public layout_policy
{
public:
    strided_layout 
    layout_from_extents(span<const std::size_t> extents) const override;

};

} // namespace multidimensional
} // namespace xmipp4
