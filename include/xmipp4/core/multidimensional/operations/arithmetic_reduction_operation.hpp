// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/reduction_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Base class for reduction operations over arithmetic types.
 *
 * `operation_shape_policy`: @c reduction_operation_shape_policy (inherited)
 * `operation_data_type_policy`: @c homogeneous_arithmetic_operation_data_type_policy
 */
class XMIPP4_CORE_API arithmetic_reduction_operation
    : public reduction_operation
{
public:
    using reduction_operation::reduction_operation;
    const operation_data_type_policy&
    get_operation_data_type_policy() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
