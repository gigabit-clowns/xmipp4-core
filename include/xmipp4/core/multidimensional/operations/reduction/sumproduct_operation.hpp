// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/arithmetic_reduction_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Compute `sum(x * y, axes)`.
 */
class XMIPP4_CORE_API sumproduct_operation final
    : public arithmetic_reduction_operation
{
public:
    using arithmetic_reduction_operation::arithmetic_reduction_operation;
    std::string get_name() const override;
    std::size_t get_output_count() const noexcept override;
    std::size_t get_input_count() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
