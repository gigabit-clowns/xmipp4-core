// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/bases/floating_elementwise_operation.hpp>

#include <xmipp4/core/multidimensional/operations/policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operations/policies/same_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const shape_policy& floating_elementwise_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& floating_elementwise_operation::get_data_type_policy() const noexcept
{
	return same_floating_data_type_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
