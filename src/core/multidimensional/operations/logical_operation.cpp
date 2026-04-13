// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/logical_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

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
