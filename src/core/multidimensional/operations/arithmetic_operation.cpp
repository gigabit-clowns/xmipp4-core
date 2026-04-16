// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic_operation.hpp>

#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/data_type_policies/same_arithmetic_data_type_policy.hpp>
#include <xmipp4/core/multidimensional/data_type_policies/same_complex_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const shape_policy& arithmetic_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& 
arithmetic_operation::get_data_type_policy() const noexcept
{
	return same_arithmetic_data_type_policy::get();
}

std::string negate_operation::get_name() const
{
	return "negate";
}

std::size_t negate_operation::get_output_count() const noexcept { return 1; }
std::size_t negate_operation::get_input_count() const noexcept { return 1; }

std::string conjugate_operation::get_name() const
{
	return "conjugate";
}

const shape_policy& conjugate_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& 
conjugate_operation::get_data_type_policy() const noexcept
{
	return same_complex_data_type_policy::get();
}

std::size_t conjugate_operation::get_output_count() const noexcept { return 1; }
std::size_t conjugate_operation::get_input_count() const noexcept { return 1; }

std::string abs_operation::get_name() const
{
	return "abs";
}

std::size_t abs_operation::get_output_count() const noexcept { return 1; }
std::size_t abs_operation::get_input_count() const noexcept { return 1; }

std::string add_operation::get_name() const
{
	return "add";
}

std::size_t add_operation::get_output_count() const noexcept { return 1; }
std::size_t add_operation::get_input_count() const noexcept { return 2; }

std::string subtract_operation::get_name() const
{
	return "subtract";
}

std::size_t subtract_operation::get_output_count() const noexcept { return 1; }
std::size_t subtract_operation::get_input_count() const noexcept { return 2; }

std::string multiply_operation::get_name() const
{
	return "multiply";
}

std::size_t multiply_operation::get_output_count() const noexcept { return 1; }
std::size_t multiply_operation::get_input_count() const noexcept { return 2; }

std::string divide_operation::get_name() const
{
	return "divide";
}

std::size_t divide_operation::get_output_count() const noexcept { return 1; }
std::size_t divide_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
