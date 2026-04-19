// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/assignment/complex_from_real_imag_operation.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/elementwise_operation_shape_policy.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/complex_from_real_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string complex_from_real_imag_operation::get_name() const
{
	return "complex_from_real_imag";
}

const operation_shape_policy& 
complex_from_real_imag_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_operation_shape_policy::get();
}

const operation_data_type_policy& 
complex_from_real_imag_operation::get_operation_data_type_policy() const noexcept
{
	return complex_from_real_operation_data_type_policy::get();
}

std::size_t 
complex_from_real_imag_operation::get_output_count() const noexcept { return 1; }
std::size_t 
complex_from_real_imag_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
