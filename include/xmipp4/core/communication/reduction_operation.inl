// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "reduction_operation.hpp"
#include "../platform/enum_helpers.hpp"

namespace xmipp4
{
namespace communication
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(reduction_operation op) noexcept
{
	switch (op)
	{
	XMIPP4_ENUM_TO_STR_CASE(reduction_operation, sum)
	XMIPP4_ENUM_TO_STR_CASE(reduction_operation, product)
	XMIPP4_ENUM_TO_STR_CASE(reduction_operation, min)
	XMIPP4_ENUM_TO_STR_CASE(reduction_operation, max)
	default: return "";
	}
}

} // namespace communication
} // namespace xmipp4
