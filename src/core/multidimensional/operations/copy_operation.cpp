// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>

#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <core/multidimensional/data_type_policies/copy_data_type_policy.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

std::string copy_operation::get_name() const
{
	return "copy";
}

const shape_policy& copy_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& copy_operation::get_data_type_policy() const noexcept
{
	return copy_data_type_policy::get();
}

std::size_t copy_operation::get_output_count() const noexcept { return 1; }
std::size_t copy_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
