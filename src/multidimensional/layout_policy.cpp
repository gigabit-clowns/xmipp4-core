// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/layout_policy.hpp>

#include <xmipp4/core/multidimensional/contiguous_layout_policy.hpp>

namespace xmipp4 
{
namespace multidimensional
{

const layout_policy& get_default_layout_policy() noexcept
{
    static const contiguous_layout_policy default_layout_policy;
    return default_layout_policy;
}

} // namespace multidimensional
} // namespace xmipp4
