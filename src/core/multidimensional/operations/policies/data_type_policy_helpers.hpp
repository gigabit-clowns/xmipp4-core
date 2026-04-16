// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/platform/compiler.h>

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

namespace xmipp4
{
namespace multidimensional
{
namespace detail
{

XMIPP4_NORETURN
inline void throw_mismatch(
	numerical_type expected,
	numerical_type got,
	const char *context
)
{
	std::ostringstream oss;
	oss << context << ": expected all operands to have data type "
		<< expected << ", but got " << got << ".";
	throw std::invalid_argument(oss.str());
}

XMIPP4_NORETURN
inline void throw_category(
	numerical_type got,
	const char *category,
	const char *context
)
{
	std::ostringstream oss;
	oss << context << ": expected all operands to have a " << category
		<< " data type, but got " << got << ".";
	throw std::invalid_argument(oss.str());
}

inline numerical_type require_same(
	span<const numerical_type> types,
	const char *context
)
{
	if (types.empty())
	{
		throw std::invalid_argument(
			std::string(context) + ": at least one operand is required."
		);
	}

	const auto reference = types[0];
	for (std::size_t i = 1; i < types.size(); ++i)
	{
		const auto &type = types[i];
		if (type != reference)
		{
			throw_mismatch(reference, type, context);
		}
	}

	return reference;
}

inline void require_exact(
	span<const numerical_type> types,
	numerical_type value,
	const char *context
)
{
	for (auto type : types)
	{
		if (type != value)
		{
			throw_mismatch(value, type, context);
		}
	}
}

inline void fill(span<numerical_type> types, numerical_type value)
{
	std::fill(types.begin(), types.end(), value);
}

} // namespace detail
} // namespace multidimensional
} // namespace xmipp4
