// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <rexlib/core/span.hpp>

#include <cstddef>
#include <vector>

namespace rexlib
{
namespace em
{

/**
 * @brief Copy the extents of a plan, checking that both sides can hold them.
 *
 * @param extents The extents of one region.
 * @param file_rank Rank of the file side.
 * @param array_rank Rank of the array side.
 * @param plan_name Name of the plan being constructed, used to prefix the
 * message of the exception thrown.
 * @return std::vector<std::size_t> The extents.
 * @throws std::invalid_argument If @p extents has more axes than either
 * @p file_rank or @p array_rank.
 */
std::vector<std::size_t> checked_extents(
	span<const std::size_t> extents,
	std::size_t file_rank,
	std::size_t array_rank,
	const char *plan_name
);

} // namespace em
} // namespace rexlib
