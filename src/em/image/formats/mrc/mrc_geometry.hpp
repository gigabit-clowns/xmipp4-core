// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "mrc_header.hpp"

#include <rexlib/core/numerical/numerical_type.hpp>
#include <rexlib/core/span.hpp>

#include <cstddef>
#include <vector>

namespace rexlib
{
namespace em
{
namespace mrc
{

/**
 * @brief What a header says about the shape of the values of a file.
 *
 * An MRC file states its shape as three counts and a space group, and the
 * same three counts mean different things depending on that space group: a
 * stack of images and one volume of the same depth differ only in it, and a
 * stack of volumes divides the sections between its two leading axes, unless
 * the division leaves one of them a single element, which is how a volume and
 * a stack of images are stated too. This resolves all of it once, when a file
 * is opened, into the extents and the core rank an @ref image_reader reports.
 *
 * The values themselves are laid out with the columns changing fastest, and
 * the header names the axis of space the columns, the rows and the sections
 * each run along. This reports them along the axes of space, so a file that
 * names them in another order is reported with the strides of its axes out of
 * descending order rather than with its axes transposed.
 */
class mrc_geometry
{
public:
	/**
	 * @brief Derive the shape of a file from its header.
	 *
	 * @param header The header of the file.
	 * @throws image_format_error If the columns, the rows and the sections of
	 * the file do not run along the three axes of space, one each, or if the
	 * values of the file would not begin at an offset its elements can be
	 * addressed at.
	 */
	explicit mrc_geometry(const mrc_header &header);

	mrc_geometry(const mrc_geometry &other) = default;
	mrc_geometry(mrc_geometry &&other) noexcept = default;
	~mrc_geometry() = default;

	mrc_geometry& operator=(const mrc_geometry &other) = default;
	mrc_geometry& operator=(mrc_geometry &&other) noexcept = default;

	/**
	 * @brief Get the extents of the file.
	 *
	 * The axes of a stack come first, and the axes of one image or volume
	 * follow in the order of the axes of space, the one along the first axis
	 * of space last.
	 *
	 * @return span<const std::size_t> The extents.
	 */
	span<const std::size_t> get_extents() const noexcept;

	/**
	 * @brief Get how many of the extents are one image or volume.
	 *
	 * @return std::size_t Two for a file of images, three for one of
	 * volumes.
	 */
	std::size_t get_core_rank() const noexcept;

	/**
	 * @brief Get the distance between consecutive elements along each axis.
	 *
	 * In elements rather than bytes, and of the same rank as the extents and
	 * in their order.
	 *
	 * @return span<const std::ptrdiff_t> The strides.
	 */
	span<const std::ptrdiff_t> get_strides() const noexcept;

	/**
	 * @brief Get the data type of the elements of the file.
	 *
	 * @return numerical_type The data type.
	 */
	numerical_type get_data_type() const noexcept;

	/**
	 * @brief Get where the values of the file begin.
	 *
	 * @return std::size_t The offset in bytes.
	 */
	std::size_t get_data_offset() const noexcept;

	/**
	 * @brief Get how many elements the file holds.
	 *
	 * @return std::size_t The product of the extents.
	 */
	std::size_t get_element_count() const noexcept;

	/**
	 * @brief Get how many bytes the values of the file occupy.
	 *
	 * @return std::size_t The size in bytes, past @ref get_data_offset.
	 */
	std::size_t get_data_size() const noexcept;

private:
	std::vector<std::size_t> m_extents;
	std::vector<std::ptrdiff_t> m_strides;
	std::size_t m_core_rank;
	numerical_type m_data_type;
	std::size_t m_data_offset;
};

} // namespace mrc
} // namespace em
} // namespace rexlib
