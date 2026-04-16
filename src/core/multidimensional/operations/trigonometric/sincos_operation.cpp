// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/trigonometric/sincos_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string sincos_operation::get_name() const
{
	return "sincos";
}

std::size_t sincos_operation::get_output_count() const noexcept { return 2; }
std::size_t sincos_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
