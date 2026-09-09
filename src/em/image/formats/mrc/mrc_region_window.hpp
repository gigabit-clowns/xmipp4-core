// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>

namespace rexlib
{
namespace em
{

class image_transfer_plan;

namespace mrc
{

class mrc_geometry;

/**
 * @brief The stretch of a file that a batch of regions reaches into.
 *
 * Byte range along the slowest axis, from the first position a batch reaches
 * to the last, gaps included, measured from the start of the values.
 *
 * This is the prefetch range, not a read bound. Regions past the end of the
 * file are clamped out here and later rejected by @ref mrc_region_read_plan.
 */
struct mrc_region_window
{
	std::size_t byte_offset;
	std::size_t byte_size;
};

/**
 * @brief Work out the stretch of a file a batch of regions reaches into.
 *
 * @param regions The regions to be moved.
 * @param geometry The shape of the file they address.
 * @return mrc_region_window The stretch, clamped to the values of the file.
 * Empty for a batch of no region.
 */
mrc_region_window make_region_window(
	const image_transfer_plan &regions,
	const mrc_geometry &geometry
) noexcept;

} // namespace mrc
} // namespace em
} // namespace rexlib
