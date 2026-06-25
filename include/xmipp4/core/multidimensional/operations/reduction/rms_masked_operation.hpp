// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/masked_reduction_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Masked variant of @ref rms_operation. The first input is a boolean
 * mask.
 */
class XMIPP4_CORE_API rms_masked_operation final
    : public masked_reduction_operation
{
public:
    using masked_reduction_operation::masked_reduction_operation;
    std::string get_name() const override;
    operation_arity get_arity() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
