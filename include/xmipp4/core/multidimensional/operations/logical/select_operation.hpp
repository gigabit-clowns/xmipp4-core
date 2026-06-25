// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Compute `mask ? x : y` for all elements.
 *
 * The mask input must be boolean. The x, y inputs and the output must share
 * the same numerical type. Similar to numpy's `where`
 */
class XMIPP4_CORE_API select_operation final
    : public operation
{
public:
    std::string get_name() const override;
    const operation_shape_policy& 
	get_operation_shape_policy() const noexcept override;
    const operation_data_type_policy& 
	get_operation_data_type_policy() const noexcept override;
    operation_arity get_arity() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
