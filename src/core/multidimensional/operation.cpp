// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

operation::operation() noexcept = default;
operation::~operation() = default;

operation_id operation::get_id() const noexcept
{
	return operation_id(typeid(*this));
}

std::string operation::serialize_parameters() const
{
	return "";
}

} // namespace multidimensional
} // namespace xmipp4
