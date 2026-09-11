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
 * This is the prefetch range for a batch @ref mrc_region_read_plan has
 * already validated: every position is assumed to already be within the
 * file, since a batch that is not is refused there before either of them
 * ever runs.
 */
class mrc_region_window
{
public:
	/**
	 * @brief Construct the stretch from its bounds, in bytes.
	 *
	 * @param byte_offset Byte offset of the first value the stretch reaches,
	 * from the start of the values.
	 * @param byte_size Number of bytes the stretch spans.
	 */
	mrc_region_window(
		std::size_t byte_offset,
		std::size_t byte_size
	) noexcept;

	mrc_region_window(const mrc_region_window &other) = default;
	mrc_region_window(mrc_region_window &&other) noexcept = default;
	~mrc_region_window() = default;

	mrc_region_window& operator=(const mrc_region_window &other) = default;
	mrc_region_window& operator=(mrc_region_window &&other) noexcept = default;

	/**
	 * @brief Get the byte offset of the first value the stretch reaches.
	 *
	 * @return std::size_t The byte offset, from the start of the values.
	 */
	std::size_t get_byte_offset() const noexcept;

	/**
	 * @brief Get the number of bytes the stretch spans.
	 *
	 * @return std::size_t The byte size.
	 */
	std::size_t get_byte_size() const noexcept;

private:
	std::size_t m_byte_offset;
	std::size_t m_byte_size;
};

/**
 * @brief Work out the stretch of a file a batch of regions reaches into.
 *
 * @param regions The regions to be moved. Every position along the slowest
 * axis is assumed to already be within @p geometry's extents.
 * @param geometry The shape of the file they address.
 * @return mrc_region_window The stretch, in bytes. Empty for a batch of no
 * region.
 */
mrc_region_window make_region_window(
	const image_transfer_plan &regions,
	const mrc_geometry &geometry
) noexcept;

} // namespace mrc
} // namespace em
} // namespace rexlib
