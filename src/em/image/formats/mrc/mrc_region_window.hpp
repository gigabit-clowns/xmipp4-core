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
 * Whole positions along the slowest axis of the file, from the first the
 * batch reaches to the last, gaps included, stated in bytes from the start of
 * the values. A batch of scattered positions therefore covers what lies
 * between them as well, which is what keeps this one stretch of the file
 * rather than a list of them.
 *
 * It says where a batch is going to read, which is all a prefetch needs. It
 * is not a bound on where a batch may read: a region past the end of the file
 * is clamped away here and refused by @ref mrc_region_read_plan, where the
 * batch is resolved and every region is checked.
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
