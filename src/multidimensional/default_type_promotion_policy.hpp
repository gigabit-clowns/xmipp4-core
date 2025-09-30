// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/type_promotion_policy.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class default_type_promotion_policy final
    : public type_promotion_policy
{
public:
    ~default_type_promotion_policy() override = default;

    void promote(
        span<const numerical_type> inputs, 
        span<numerical_type> outputs
    ) const override;

};

} // namespace multidimensional
} // namespace xmipp4

