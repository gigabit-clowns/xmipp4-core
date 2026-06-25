// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/linear_algebra_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Matrix-matrix multiplication: C = A * B
 *
 * Supports batched multiplication over leading dimensions.
 *
 * input[0]: (..., m, k) - left-hand matrix (or batch of matrices)
 * input[1]: (..., k, n) - right-hand matrix (or batch of matrices)
 * output:   (..., m, n) - result
 */
class XMIPP4_CORE_API matmul_operation final
    : public linear_algebra_operation
{
public:
    std::string get_name() const override;
    operation_arity get_arity() const noexcept override;
    const operation_shape_policy& 
	get_operation_shape_policy() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
