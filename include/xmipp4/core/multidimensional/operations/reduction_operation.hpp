// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operation_shape_policies/reduction_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/span.hpp>

#include <cstddef>
#include <vector>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Base class for reduction operations.
 *
 * Stores a @ref reduction_operation_shape_policy parameterised by the
 * provided axes and @p keep_dim flag. Derived classes supply the data type
 * policy.
 */
class XMIPP4_CORE_API reduction_operation
    : public operation
{
public:
    /**
     * @param axes Axes along which the reduction is performed. An empty list
     * means reduce over all dimensions.
     * @param keep_dim When true, reduced axes are retained with extent 1.
     */
    explicit reduction_operation(
        std::vector<std::size_t> axes,
        bool keep_dim = false
    );
    ~reduction_operation() override;

    const operation_shape_policy&
    get_operation_shape_policy() const noexcept override;

    /**
     * @brief Return the sorted, deduplicated reduction axes.
     */
    span<const std::size_t> get_reduction_axes() const noexcept;

    /**
     * @brief Return whether reduced dimensions are retained with extent 1.
     */
    bool keeps_dimensions() const noexcept;

private:
    reduction_operation_shape_policy m_shape_policy;
};

} // namespace multidimensional
} // namespace xmipp4
