// SPDX-License-Identifier: GPL-3.0-only

#include "default_type_promotion_policy.hpp"

namespace xmipp4 
{
namespace multidimensional
{

void default_type_promotion_policy::promote(
    span<const numerical_type> inputs, 
    span<numerical_type> outputs
) const
{
    if (outputs.empty()) 
    {
        return;
    }

    if (inputs.empty()) 
    {
        throw std::invalid_argument(
            "At least one input type is required to deduce the output type."
        );
    }

    auto result_type = inputs[0];
    for (std::size_t i = 1; i < inputs.size(); ++i) 
    {
        result_type = common_type(result_type, inputs[i]);
    }

    for (std::size_t i = 0; i < outputs.size(); ++i) 
    {
        outputs[i] = result_type;
    }
}

} // namespace multidimensional
} // namespace xmipp4
