// SPDX-License-Identifier: GPL-3.0-only

#include "operation_shape_policy_helpers.hpp"

#include <sstream>

namespace xmipp4
{
namespace multidimensional
{

inline
void require_1d(
	const std::vector<std::size_t> &shape, 
	const char *context,
	const char *operand_name
)
{
	if (shape.size() != 1)
	{
		std::ostringstream oss;
		oss << context << " requires " << operand_name << " operand to be 1D";
		throw std::invalid_argument(oss.str());
	}
}

inline
void require_2d(
	const std::vector<std::size_t> &shape, 
	const char *context,
	const char *operand_name
)
{
	if (shape.size() != 2)
	{
		std::ostringstream oss;
		oss << context << " requires " << operand_name << " operand to be 2D";
		throw std::invalid_argument(oss.str());
	}
}

inline
void require_at_least_1d(
	const std::vector<std::size_t> &shape, 
	const char *context,
	const char *operand_name
)
{
	if (shape.size() < 1)
	{
		std::ostringstream oss;
		oss << context << " requires " << operand_name 
			<< " operand to be at least 1D";
		throw std::invalid_argument(oss.str());
	}
}

inline
void require_at_least_2d(
	const std::vector<std::size_t> &shape, 
	const char *context,
	const char *operand_name
)
{
	if (shape.size() < 2)
	{
		std::ostringstream oss;
		oss << context << " requires " << operand_name 
			<< " operand to be at least 2D";
		throw std::invalid_argument(oss.str());
	}
}

} // namespace multidimensional
} // namespace xmipp4
