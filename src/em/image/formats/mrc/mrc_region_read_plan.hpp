// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "mrc_region_offsets.hpp"

#include <rexlib/core/layout/joint_layout.hpp>

namespace rexlib
{
namespace em
{
namespace mrc
{

/**
 * @brief A batch of regions to be read, ready to be walked.
 *
 * A read writes the array, so the array is named first in the layout.
 * That is what settles the order the axes are walked in, and so makes the
 * traversal sequential in the array.
 *
 * That is the whole of what separates this from
 * @ref mrc_region_write_plan, and it is why the two are separate types
 * rather than one carrying a direction: a batch resolved for one of them
 * states nothing that would make it correct for the other, and only
 * @ref read_regions accepts this one.
 */
class mrc_region_read_plan
{
public:
	/**
	 * @brief Resolve a batch of regions and build the space to walk it in.
	 *
	 * @param regions The regions to move.
	 * @param file_extents Extents of the file.
	 * @param file_strides Distance between consecutive elements of the file
	 * along each axis, in elements.
	 * @param array_extents Extents of the array.
	 * @param array_strides Distance between consecutive elements of the
	 * array along each axis, in elements.
	 * @param array_offset Index of the first element of the array.
	 * @throws std::invalid_argument If a rank does not match the batch or
	 * the strides do not match their extents.
	 * @throws std::out_of_range If a region is not contained in the file or
	 * in the array where it is placed.
	 */
	mrc_region_read_plan(
		const image_transfer_plan &regions,
		span<const std::size_t> file_extents,
		span<const std::ptrdiff_t> file_strides,
		span<const std::size_t> array_extents,
		span<const std::ptrdiff_t> array_strides,
		std::ptrdiff_t array_offset
	);

	mrc_region_read_plan(const mrc_region_read_plan &other) = delete;
	mrc_region_read_plan(mrc_region_read_plan &&other) noexcept = default;
	~mrc_region_read_plan() = default;

	mrc_region_read_plan&
	operator=(const mrc_region_read_plan &other) = delete;
	mrc_region_read_plan&
	operator=(mrc_region_read_plan &&other) noexcept = default;

	/**
	 * @brief Get where each region starts on each side.
	 *
	 * @return const mrc_region_offsets& The offsets.
	 */
	const mrc_region_offsets& get_offsets() const noexcept;

	/**
	 * @brief Get the space every region is walked in.
	 *
	 * @return const joint_layout& The layout, naming the array first.
	 */
	const joint_layout& get_layout() const noexcept;

private:
	mrc_region_offsets m_offsets;
	joint_layout m_layout;
};

} // namespace mrc
} // namespace em
} // namespace rexlib
