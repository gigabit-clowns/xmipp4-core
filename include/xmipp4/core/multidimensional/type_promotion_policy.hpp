// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class type_promotion_policy
{
public:
    virtual ~type_promotion_policy() = default;

    virtual 
    void promote(
        span<const numerical_type> inputs, 
        span<numerical_type> outputs
    ) const = 0;

};

const type_promotion_policy & get_default_type_promotion_policy() noexcept;

} // namespace multidimensional
} // namespace xmipp4

