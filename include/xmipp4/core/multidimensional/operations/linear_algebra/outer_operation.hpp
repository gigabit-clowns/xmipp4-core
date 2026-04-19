// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/linear_algebra_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Outer product of two 1D vectors: result[i, j] = x[i] * y[j]
 *
 * input[0]: (m,)   - first vector
 * input[1]: (n,)   - second vector
 * output:   (m, n) - result matrix
 */
class XMIPP4_CORE_API outer_operation final
    : public linear_algebra_operation
{
public:
    std::string get_name() const override;
    std::size_t get_output_count() const noexcept override;
    std::size_t get_input_count() const noexcept override;
    const operation_shape_policy& get_operation_shape_policy() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
