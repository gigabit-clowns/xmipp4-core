// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Create a complex array from the real and imaginary values.
 */
class XMIPP4_CORE_API complex_from_real_imag_operation final
	: public operation
{
public:
	std::string get_name() const override;
	operation_arity get_arity() const noexcept override;
	const operation_shape_policy& 
	get_operation_shape_policy() const noexcept override;
	const operation_data_type_policy& 
	get_operation_data_type_policy() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
