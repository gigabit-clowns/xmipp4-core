// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/logical_operation.hpp>

#include <xmipp4/core/multidimensional/operations/policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operations/policies/boolean_data_type_policy.hpp>

namespace xmipp4 
{
namespace multidimensional
{

const shape_policy& logical_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& logical_operation::get_data_type_policy() const noexcept
{
	return boolean_data_type_policy::get();
}

std::string logical_negate_operation::get_name() const
{
	return "logical_negate";
}

std::string logical_and_operation::get_name() const
{
	return "logical_and";
}

std::string logical_or_operation::get_name() const
{
	return "logical_or";
}

} // namespace multidimensional
} // namespace xmipp4
