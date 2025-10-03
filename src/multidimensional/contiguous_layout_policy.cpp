// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/contiguous_layout_policy.hpp>

namespace xmipp4 
{
namespace multidimensional
{

strided_layout 
contiguous_layout_policy::layout_from_extents(span<const std::size_t> extents) const
{
    return strided_layout::make_contiguous_layout(extents);
}

} // namespace multidimensional
} // namespace xmipp4
