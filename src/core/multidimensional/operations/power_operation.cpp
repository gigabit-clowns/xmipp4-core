// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/power_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string power_operation::get_name() const
{
	return "power";
}

std::size_t power_operation::get_output_count() const noexcept { return 1; }
std::size_t power_operation::get_input_count() const noexcept { return 2; }

std::string exp_operation::get_name() const
{
	return "exp";
}

std::size_t exp_operation::get_output_count() const noexcept { return 1; }
std::size_t exp_operation::get_input_count() const noexcept { return 1; }

std::string log_operation::get_name() const
{
	return "log";
}

std::size_t log_operation::get_output_count() const noexcept { return 1; }
std::size_t log_operation::get_input_count() const noexcept { return 1; }

std::string square_operation::get_name() const
{
	return "square";
}

std::size_t square_operation::get_output_count() const noexcept { return 1; }
std::size_t square_operation::get_input_count() const noexcept { return 1; }

std::string sqrt_operation::get_name() const
{
	return "sqrt";
}

std::size_t sqrt_operation::get_output_count() const noexcept { return 1; }
std::size_t sqrt_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
