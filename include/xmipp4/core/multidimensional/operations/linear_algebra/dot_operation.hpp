// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/linear_algebra_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Dot product of two 1D vectors: result = sum(x * y)
 *
 * input[0]: (n,) - first vector
 * input[1]: (n,) - second vector of the same length
 * output:   ()   - scalar result (rank 0)
 */
class XMIPP4_CORE_API dot_operation final
    : public linear_algebra_operation
{
public:
    std::string get_name() const override;
    std::size_t get_output_count() const noexcept override;
    std::size_t get_input_count() const noexcept override;
    const shape_policy& get_shape_policy() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
