// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/trigonometric_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string sin_operation::get_name() const
{
	return "sin";
}

std::size_t sin_operation::get_output_count() const noexcept { return 1; }
std::size_t sin_operation::get_input_count() const noexcept { return 1; }

std::string cos_operation::get_name() const
{
	return "cos";
}

std::size_t cos_operation::get_output_count() const noexcept { return 1; }
std::size_t cos_operation::get_input_count() const noexcept { return 1; }

std::string sincos_operation::get_name() const
{
	return "sincos";
}

std::size_t sincos_operation::get_output_count() const noexcept { return 2; }
std::size_t sincos_operation::get_input_count() const noexcept { return 1; }

std::string tan_operation::get_name() const
{
	return "tan";
}

std::size_t tan_operation::get_output_count() const noexcept { return 1; }
std::size_t tan_operation::get_input_count() const noexcept { return 1; }

std::string asin_operation::get_name() const
{
	return "asin";
}

std::size_t asin_operation::get_output_count() const noexcept { return 1; }
std::size_t asin_operation::get_input_count() const noexcept { return 1; }

std::string acos_operation::get_name() const
{
	return "acos";
}

std::size_t acos_operation::get_output_count() const noexcept { return 1; }
std::size_t acos_operation::get_input_count() const noexcept { return 1; }

std::string atan_operation::get_name() const
{
	return "atan";
}

std::size_t atan_operation::get_output_count() const noexcept { return 1; }
std::size_t atan_operation::get_input_count() const noexcept { return 1; }

std::string atan2_operation::get_name() const
{
	return "atan2";
}

std::size_t atan2_operation::get_output_count() const noexcept { return 1; }
std::size_t atan2_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
