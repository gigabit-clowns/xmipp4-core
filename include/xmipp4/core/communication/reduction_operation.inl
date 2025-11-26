// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "reduction_operation.hpp"

namespace xmipp4
{
namespace communication
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(reduction_operation op) noexcept
{
	switch (op)
	{
	case reduction_operation::sum: return "sum";
	case reduction_operation::product: return "product";
	case reduction_operation::min: return "min";
	case reduction_operation::max: return "max";
	default: return "";
	}
}

} // namespace communication
} // namespace xmipp4
