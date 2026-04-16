// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class for floating point elementwise operations.
 * 
 * `shape_policy`: `elementwise_shape_policy`
 * `data_type_policy`: `same_floating_data_type_policy`
 */
class XMIPP4_CORE_API floating_elementwise_operation
	: public operation
{
public:
	const shape_policy& get_shape_policy() const noexcept override;
	const data_type_policy& get_data_type_policy() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4

