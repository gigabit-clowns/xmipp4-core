// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/linear_algebra_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Matrix-vector multiplication: y = A * x
 *
 * input[0]: (m, n) - matrix
 * input[1]: (n,)   - input vector
 * output:   (m,)   - result vector
 */
class XMIPP4_CORE_API matvec_operation final
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
