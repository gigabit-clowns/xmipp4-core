// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <rexlib/core/layout/joint_layout.hpp>
#include <rexlib/core/memory/byte.hpp>
#include <rexlib/core/memory/byte_order.hpp>
#include <rexlib/core/numerical/numerical_type.hpp>
#include <rexlib/core/span.hpp>

#include <cstddef>
#include <vector>

namespace rexlib
{
namespace em
{

class image_transfer_plan;

namespace mrc
{

/**
 * @brief One batch of regions moved between a mapped file and an array.
 *
 * Every region of a plan has the same extents and differs only in where it
 * starts on each side, so the iteration space is the same for all of them:
 * one @ref joint_layout is built here and every region is walked with it,
 * differing only by a pointer. Building it once is what keeps the per region
 * cost to arithmetic, whatever the batch size.
 *
 * The layout names the array first and the file second, in both directions.
 * What changes between a read and a write is which of the two is written
 * through and which side the byte order is converted on, not the geometry, so
 * one instance serves either.
 *
 * Where each region starts is resolved into a pair of pointer offsets when
 * the transfer is constructed, and so is every bounds check. A plan that does
 * not fit is refused before anything has been moved rather than halfway
 * through it.
 */
class mrc_region_transfer
{
public:
	/**
	 * @brief Resolve a batch of regions against the two sides they address.
	 *
	 * The extents of @p regions cover the trailing axes of each side, which
	 * spans a single position along the leading axes they do not reach.
	 *
	 * @param regions The regions to move.
	 * @param file_extents Extents of the file.
	 * @param file_strides Distance between consecutive elements of the file
	 * along each axis, in elements.
	 * @param array_extents Extents of the array.
	 * @param array_strides Distance between consecutive elements of the
	 * array along each axis, in elements.
	 * @param array_offset Index of the first element of the array.
	 * @throws std::invalid_argument If a rank does not match the plan or the
	 * strides do not match their extents.
	 * @throws std::out_of_range If a region is not contained in the file or
	 * in the array where it is placed.
	 */
	mrc_region_transfer(
		const image_transfer_plan &regions,
		span<const std::size_t> file_extents,
		span<const std::ptrdiff_t> file_strides,
		span<const std::size_t> array_extents,
		span<const std::ptrdiff_t> array_strides,
		std::ptrdiff_t array_offset
	);

	mrc_region_transfer(const mrc_region_transfer &other) = delete;
	mrc_region_transfer(mrc_region_transfer &&other) noexcept;
	~mrc_region_transfer();

	mrc_region_transfer&
	operator=(const mrc_region_transfer &other) = delete;
	mrc_region_transfer&
	operator=(mrc_region_transfer &&other) noexcept;

	/**
	 * @brief Get how many regions are moved.
	 *
	 * @return std::size_t The number of regions.
	 */
	std::size_t get_region_count() const noexcept;

	/**
	 * @brief Move every region out of the file and into the array.
	 *
	 * Values are converted to @p array_type, and read in @p file_order.
	 *
	 * @param array_data First element of the array.
	 * @param array_type Data type of the array.
	 * @param file_data First element of the values of the file, past both
	 * of its headers.
	 * @param file_type Data type of the file.
	 * @param file_order Byte order the file states its values in.
	 * @throws invalid_operation_error If @p array_type can not be produced
	 * from @p file_type, or if @p file_type is not one an MRC file holds.
	 */
	void read(
		void *array_data,
		numerical_type array_type,
		const byte *file_data,
		numerical_type file_type,
		byte_order file_order
	) const;

	/**
	 * @brief Move every region out of the array and into the file.
	 *
	 * The mirror of @ref read: the same regions and the same layout, the
	 * opposite direction.
	 *
	 * @param array_data First element of the array.
	 * @param array_type Data type of the array.
	 * @param file_data First element of the values of the file, past both
	 * of its headers.
	 * @param file_type Data type of the file.
	 * @param file_order Byte order the file states its values in.
	 * @throws invalid_operation_error If @p file_type can not be produced
	 * from @p array_type, or if @p file_type is not one an MRC file holds.
	 */
	void write(
		const void *array_data,
		numerical_type array_type,
		byte *file_data,
		numerical_type file_type,
		byte_order file_order
	) const;

private:
	joint_layout m_layout;
	std::vector<std::ptrdiff_t> m_array_offsets;
	std::vector<std::ptrdiff_t> m_file_offsets;
};

/**
 * @brief Move every region of one batch out of a file of one element type.
 *
 * Defined in mrc_region_transfer_impl.hpp and explicitly instantiated once
 * per element type an MRC file holds, in the mrc_region_transfer_<type>.cpp
 * files. The array side is dispatched over every data type there is, and each
 * pair of types drives a loop specialized on the strides of both operands, so
 * one element type is already a grid the size of the CPU copy builder's. A
 * translation unit per element type keeps each of them well inside it.
 *
 * @tparam Q Element type of the file.
 * @param layout The iteration space every region shares.
 * @param array_offsets Where each region starts in the array, in elements.
 * @param file_offsets Where each region starts in the file, in elements.
 * @param array_data First element of the array.
 * @param array_type Data type of the array.
 * @param file_data First element of the values of the file.
 * @param swapped Whether the file states its values in the other byte order.
 * @throws invalid_operation_error If @p array_type can not be produced from
 * @p Q.
 */
template <typename Q>
void read_regions(
	const joint_layout &layout,
	const std::vector<std::ptrdiff_t> &array_offsets,
	const std::vector<std::ptrdiff_t> &file_offsets,
	void *array_data,
	numerical_type array_type,
	const Q *file_data,
	bool swapped
);

/**
 * @brief Move every region of one batch into a file of one element type.
 *
 * The mirror of @ref read_regions, split across translation units the same
 * way and for the same reason.
 *
 * @see read_regions
 */
template <typename Q>
void write_regions(
	const joint_layout &layout,
	const std::vector<std::ptrdiff_t> &array_offsets,
	const std::vector<std::ptrdiff_t> &file_offsets,
	const void *array_data,
	numerical_type array_type,
	Q *file_data,
	bool swapped
);

} // namespace mrc
} // namespace em
} // namespace rexlib
