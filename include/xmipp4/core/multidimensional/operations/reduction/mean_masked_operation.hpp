// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/masked_reduction_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Masked variant of @ref mean_operation. The first input is a boolean
 * mask.
 */
class XMIPP4_CORE_API mean_masked_operation final
    : public masked_reduction_operation
{
public:
    using masked_reduction_operation::masked_reduction_operation;
    std::string get_name() const override;
    std::size_t get_output_count() const noexcept override;
    std::size_t get_input_count() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
