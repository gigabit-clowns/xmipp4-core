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
 * @brief Check the extents of a region fit both sides of a plan and copy
 * them.
 *
 * @ref image_transfer_plan and @ref image_transaction_plan are constructed
 * from the same shape: extents that must not outrank either side of the
 * plan they describe. This is that one check, shared so neither carries its
 * own copy of it.
 *
 * @param extents Extents of one region, as given to the constructor of the
 * calling plan.
 * @param file_rank Rank of the file side of the plan.
 * @param array_rank Rank of the array side of the plan.
 * @param file_rank_message Message thrown if @p extents outranks
 * @p file_rank.
 * @param array_rank_message Message thrown if @p extents outranks
 * @p array_rank.
 * @return std::vector<std::size_t> A copy of @p extents.
 * @throws std::invalid_argument With @p file_rank_message or
 * @p array_rank_message, whichever check failed.
 */
std::vector<std::size_t> sanitize_region_extents(
	span<const std::size_t> extents,
	std::size_t file_rank,
	std::size_t array_rank,
	const char *file_rank_message,
	const char *array_rank_message
);

} // namespace em
} // namespace rexlib
