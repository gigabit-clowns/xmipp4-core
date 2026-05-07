// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/floating_reduction_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Compute `sum(square(x), axes) / N` (mean of squares).
 */
class XMIPP4_CORE_API mean_square_operation final
    : public floating_reduction_operation
{
public:
    using floating_reduction_operation::floating_reduction_operation;
    std::string get_name() const override;
    std::size_t get_output_count() const noexcept override;
    std::size_t get_input_count() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
