// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/operations/operation.hpp>

#include <sstream>

namespace xmipp4
{
namespace operations
{

operation::operation() noexcept = default;
operation::~operation() = default;

operation_id operation::get_id() const noexcept
{
	return operation_id(typeid(*this));
}

} // namespace operations
} // namespace xmipp4
