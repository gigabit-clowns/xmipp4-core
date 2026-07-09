// SPDX-License-Identifier: GPL-3.0-only

#include "data_type_policy_helpers.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

namespace xmipp4
{
namespace dispatch
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

inline void require_valid(
	numerical_type type,
	const char *context
)
{
	if (get_size(type) == 0)
	{
		std::ostringstream oss;
		oss << context << ": expected a valid numerical type, but got "
			<< type << ".";
		throw std::invalid_argument(oss.str());
	}
}

inline void require_category(
	numerical_type type,
	std::initializer_list<numerical_type_category> allowed,
	const char *category_name,
	const char *context
)
{
	const auto category = get_category(type);
	for (auto a : allowed)
	{
		if (category == a)
		{
			return;
		}
	}
	throw_category(type, category_name, context);
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

} // namespace dispatch
} // namespace xmipp4
