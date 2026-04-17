// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Base class for linear algebra operations.
 *
 * `data_type_policy`: `same_arithmetic_data_type_policy`
 *
 * Subclasses must provide a `shape_policy` appropriate to their geometry
 * (e.g., matmul, matvec, dot, outer).
 */
class XMIPP4_CORE_API linear_algebra_operation
    : public operation
{
public:
    const data_type_policy& get_data_type_policy() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
