// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/bitwise_operation.hpp>

#include <xmipp4/core/multidimensional/operations/policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operations/policies/same_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const shape_policy& bitwise_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& bitwise_operation::get_data_type_policy() const noexcept
{
	return same_integer_data_type_policy::get();
}

std::string bitwise_negate_operation::get_name() const
{
	return "bitwise_negate";
}

std::string bitwise_and_operation::get_name() const
{
	return "bitwise_and";
}

std::string bitwise_or_operation::get_name() const
{
	return "bitwise_or";
}

std::string bitwise_xor_operation::get_name() const
{
	return "bitwise_xor";
}

} // namespace multidimensional
} // namespace xmipp4
