// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Base class for arithmetic operations.
 *
 * `operation_shape_policy`: `elementwise_operation_shape_policy`
 * `operation_data_type_policy`: `homogeneous_arithmetic_operation_data_type_policy`
 */
class XMIPP4_CORE_API arithmetic_operation
	: public operation
{
public:
	const operation_shape_policy& get_operation_shape_policy() const noexcept override;
	const operation_data_type_policy& get_operation_data_type_policy() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
