// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/type_promotion_policy.hpp>

#include "default_type_promotion_policy.hpp"

namespace xmipp4 
{
namespace multidimensional
{

const type_promotion_policy& get_default_type_promotion_policy() noexcept
{
    static const default_type_promotion_policy default_type_promotion_policy;
    return default_type_promotion_policy;
}

} // namespace multidimensional
} // namespace xmipp4
