// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy for operations accepting a mask as the first 
 * argument.
 *
 * The first input must be boolean. The remaining inputs and the output
 * must share the same type, which may be any valid numerical type.
 */
class XMIPP4_CORE_API masked_operation_data_type_policy final
    : public operation_data_type_policy
{
public:
    void deduce(
        span<numerical_type> canonical_output_types,
        span<const numerical_type> input_types
    ) const override;

    static const masked_operation_data_type_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
