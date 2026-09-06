// SPDX-License-Identifier: GPL-3.0-only

#include <em/image/checked_extents.hpp>

#include <stdexcept>
#include <string>

namespace rexlib
{
namespace em
{

std::vector<std::size_t> checked_extents(
	span<const std::size_t> extents,
	std::size_t file_rank,
	std::size_t array_rank,
	const char *plan_name
)
{
	if (extents.size() > file_rank)
	{
		throw std::invalid_argument(
			std::string(plan_name) +
			": The regions do not fit in the file rank."
		);
	}

	if (extents.size() > array_rank)
	{
		throw std::invalid_argument(
			std::string(plan_name) +
			": The regions do not fit in the array rank."
		);
	}

	return std::vector<std::size_t>(extents.begin(), extents.end());
}

} // namespace em
} // namespace rexlib
