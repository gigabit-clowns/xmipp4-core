// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/bitwise_operation.hpp>

#include <xmipp4/core/multidimensional/operations/policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operations/policies/same_integer_data_type_policy.hpp>

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

std::size_t 
bitwise_negate_operation::get_output_count() const noexcept { return 1; }
std::size_t 
bitwise_negate_operation::get_input_count() const noexcept { return 1; }

std::string bitwise_and_operation::get_name() const
{
	return "bitwise_and";
}

std::size_t 
bitwise_and_operation::get_output_count() const noexcept { return 1; }
std::size_t 
bitwise_and_operation::get_input_count() const noexcept { return 2; }

std::string bitwise_or_operation::get_name() const
{
	return "bitwise_or";
}

std::size_t 
bitwise_or_operation::get_output_count() const noexcept { return 1; }
std::size_t 
bitwise_or_operation::get_input_count() const noexcept { return 2; }

std::string bitwise_xor_operation::get_name() const
{
	return "bitwise_xor";
}

std::size_t 
bitwise_xor_operation::get_output_count() const noexcept { return 1; }
std::size_t 
bitwise_xor_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
