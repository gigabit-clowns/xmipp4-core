// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic/conjugate_operation.hpp>

#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/data_type_policies/same_complex_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string conjugate_operation::get_name() const
{
	return "conjugate";
}

const shape_policy& conjugate_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& conjugate_operation::get_data_type_policy() const noexcept
{
	return same_complex_data_type_policy::get();
}

std::size_t conjugate_operation::get_output_count() const noexcept { return 1; }
std::size_t conjugate_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
