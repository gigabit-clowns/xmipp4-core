// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/bitwise_operation.hpp>

#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/data_type_policies/same_integer_data_type_policy.hpp>

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

} // namespace multidimensional
} // namespace xmipp4
