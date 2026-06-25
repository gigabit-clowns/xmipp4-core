// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/floating_reduction_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Compute `sqrt(sum(square(x), axes) / N)` (root-mean-square).
 */
class XMIPP4_CORE_API rms_operation final
    : public floating_reduction_operation
{
public:
    using floating_reduction_operation::floating_reduction_operation;
    std::string get_name() const override;
    operation_arity get_arity() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
