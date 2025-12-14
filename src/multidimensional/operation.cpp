// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

operation::operation() noexcept = default;
operation::~operation() = default;

std::string operation::serialize_parameters() const
{
	return "";
}

} // namespace multidimensional
} // namespace xmipp4
