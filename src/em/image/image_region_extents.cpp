// SPDX-License-Identifier: GPL-3.0-only

#include "image_region_extents.hpp"

#include <stdexcept>

namespace rexlib
{
namespace em
{

std::vector<std::size_t> sanitize_region_extents(
	span<const std::size_t> extents,
	std::size_t file_rank,
	std::size_t array_rank,
	const char *file_rank_message,
	const char *array_rank_message
)
{
	if (extents.size() > file_rank)
	{
		throw std::invalid_argument(file_rank_message);
	}

	if (extents.size() > array_rank)
	{
		throw std::invalid_argument(array_rank_message);
	}

	return std::vector<std::size_t>(extents.begin(), extents.end());
}

} // namespace em
} // namespace rexlib
