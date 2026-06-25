// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/floating_elementwise_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Compute `trunc(x)` for all elements (round towards zero).
 */
class XMIPP4_CORE_API trunc_operation final
    : public floating_elementwise_operation
{
public:
    std::string get_name() const override;
    operation_arity get_arity() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
