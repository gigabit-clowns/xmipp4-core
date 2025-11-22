// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "reduction_operation.hpp"

#include <unordered_map>

namespace xmipp4
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

inline
bool from_string(std::string_view str, reduction_operation& op) noexcept
{
	static const
	std::unordered_map<std::string_view, reduction_operation> str_2_reduction_operation = 
	{
		{ to_string(reduction_operation::sum), reduction_operation::sum },
		{ to_string(reduction_operation::product), reduction_operation::product },
		{ to_string(reduction_operation::min), reduction_operation::min },
		{ to_string(reduction_operation::max), reduction_operation::max },

	};

	const auto ite = str_2_reduction_operation.find(str);
	const bool result = ite != str_2_reduction_operation.end();
	if(result)
		op = ite->second;
	return result;
}

} // namespace xmipp4
