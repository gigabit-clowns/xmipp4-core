// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>

namespace xmipp4 
{
namespace multidimensional
{

copy_operation::copy_operation() noexcept = default;
copy_operation::~copy_operation()  = default;

std::string copy_operation::get_name() const
{
	return "copy";
}

} // namespace multidimensional
} // namespace xmipp4
