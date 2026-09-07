// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "mrc_constants.hpp"
#include "mrc_mode.hpp"

#include <rexlib/core/memory/byte.hpp>
#include <rexlib/core/memory/byte_order.hpp>
#include <rexlib/core/span.hpp>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace rexlib
{
namespace em
{
namespace mrc
{

/**
 * @brief The main header of one MRC file.
 *
 * Holds the fields this format reads and writes, in host byte order, with the
 * byte order the file states them in. The remaining fields of the 1024 bytes
 * are ignored when a header is parsed and written as zeros when one is
 * serialized, since nothing here interprets them.
 *
 * A header is a parsed result and not the bytes themselves:
 * @ref parse_header reads it field by field at the offsets the specification
 * gives, which is what lets the byte order be discovered from the middle of
 * the block rather than assumed by a cast.
 */
class mrc_header
{
public:
	using vector_type = std::array<float, vector_size>;

	/**
	 * @brief Construct a header of zeros in the byte order of the host.
	 */
	mrc_header() noexcept;

	mrc_header(const mrc_header &other) = default;
	mrc_header(mrc_header &&other) noexcept = default;
	~mrc_header() = default;

	mrc_header& operator=(const mrc_header &other) = default;
	mrc_header& operator=(mrc_header &&other) noexcept = default;

	byte_order get_byte_order() const noexcept;
	void set_byte_order(byte_order order) noexcept;

	std::int32_t get_column_count() const noexcept;
	void set_column_count(std::int32_t count) noexcept;

	std::int32_t get_row_count() const noexcept;
	void set_row_count(std::int32_t count) noexcept;

	std::int32_t get_section_count() const noexcept;
	void set_section_count(std::int32_t count) noexcept;

	mrc_mode get_mode() const noexcept;
	void set_mode(mrc_mode mode) noexcept;

	std::int32_t get_column_sampling() const noexcept;
	void set_column_sampling(std::int32_t sampling) noexcept;

	std::int32_t get_row_sampling() const noexcept;
	void set_row_sampling(std::int32_t sampling) noexcept;

	std::int32_t get_section_sampling() const noexcept;
	void set_section_sampling(std::int32_t sampling) noexcept;

	const vector_type& get_cell_size() const noexcept;
	void set_cell_size(const vector_type &size) noexcept;

	const vector_type& get_cell_angles() const noexcept;
	void set_cell_angles(const vector_type &angles) noexcept;

	std::int32_t get_column_axis() const noexcept;
	void set_column_axis(std::int32_t axis) noexcept;

	std::int32_t get_row_axis() const noexcept;
	void set_row_axis(std::int32_t axis) noexcept;

	std::int32_t get_section_axis() const noexcept;
	void set_section_axis(std::int32_t axis) noexcept;

	float get_data_min() const noexcept;
	void set_data_min(float value) noexcept;

	float get_data_max() const noexcept;
	void set_data_max(float value) noexcept;

	float get_data_mean() const noexcept;
	void set_data_mean(float value) noexcept;

	float get_data_rms() const noexcept;
	void set_data_rms(float value) noexcept;

	std::int32_t get_space_group() const noexcept;
	void set_space_group(std::int32_t space_group) noexcept;

	std::int32_t get_extended_header_size() const noexcept;
	void set_extended_header_size(std::int32_t size) noexcept;

	std::int32_t get_version() const noexcept;
	void set_version(std::int32_t version) noexcept;

	std::int32_t get_imod_stamp() const noexcept;
	void set_imod_stamp(std::int32_t stamp) noexcept;

	std::int32_t get_imod_flags() const noexcept;
	void set_imod_flags(std::int32_t flags) noexcept;

	/**
	 * @brief Get the labels the file carries.
	 *
	 * The ones in use, in order. A file states how many it uses and holds
	 * room for @ref label_count; holding only those in use is what keeps the
	 * count and the labels from disagreeing, and keeps a blank one from
	 * appearing between two that are filled.
	 *
	 * @return span<const std::string> The labels.
	 */
	span<const std::string> get_labels() const noexcept;

	/**
	 * @brief Append a label.
	 *
	 * This is how software records that it wrote a file.
	 *
	 * @param label The text. Must be printable ASCII and must fit in
	 * @ref size::label bytes.
	 * @throws std::invalid_argument If @p label is too long or holds
	 * anything but printable ASCII.
	 * @throws std::out_of_range If the file already carries
	 * @ref label_count labels.
	 */
	void add_label(const std::string &label);

private:
	byte_order m_byte_order;
	std::int32_t m_column_count;
	std::int32_t m_row_count;
	std::int32_t m_section_count;
	mrc_mode m_mode;
	std::int32_t m_column_sampling;
	std::int32_t m_row_sampling;
	std::int32_t m_section_sampling;
	vector_type m_cell_size;
	vector_type m_cell_angles;
	std::int32_t m_column_axis;
	std::int32_t m_row_axis;
	std::int32_t m_section_axis;
	float m_data_min;
	float m_data_max;
	float m_data_mean;
	float m_data_rms;
	std::int32_t m_space_group;
	std::int32_t m_extended_header_size;
	std::int32_t m_version;
	std::int32_t m_imod_stamp;
	std::int32_t m_imod_flags;
	std::vector<std::string> m_labels;
};

/**
 * @brief Check whether a block of bytes starts an MRC file.
 *
 * Matches the leading bytes of @ref map_id at @ref offset::map, which is what
 * recognizes a file without parsing it.
 *
 * @param bytes The leading bytes of the file. Shorter than the identifier
 * reaches yields false.
 * @return bool true if the identifier is there.
 */
bool has_map_identifier(span<const byte> bytes) noexcept;

/**
 * @brief Parse the main header of an MRC file.
 *
 * The byte order is taken from the machine stamp, and then from the mode when
 * the stamp does not resolve or contradicts it: a mode that only parses the
 * other way around says the stamp is wrong, which happens in files written by
 * software that byte swapped the data and not the stamp.
 *
 * @param bytes The bytes of the file, of which the first
 * @ref header_size are read.
 * @return mrc_header The parsed header.
 * @throws image_format_error If there are not enough bytes, if the identifier
 * is missing, if no byte order resolves the mode, or if a field contradicts
 * what the format allows.
 */
mrc_header parse_header(span<const byte> bytes);

/**
 * @brief Write the main header of an MRC file.
 *
 * Writes @ref header_size bytes, zeroing every field the header does not
 * hold. The machine stamp and every numeric field are written in the byte
 * order the header states.
 *
 * @param header The header to write.
 * @param bytes Where it is written. Must be at least @ref header_size long.
 * @throws std::invalid_argument If @p bytes is too short.
 */
void serialize_header(const mrc_header &header, span<byte> bytes);

/**
 * @brief Check whether mode 0 of a header holds signed values.
 *
 * True unless the header carries the IMOD stamp without its signed byte flag,
 * which is how a file states that its bytes are unsigned.
 *
 * @param header The header to consult.
 * @return bool true if mode 0 holds signed values.
 */
bool holds_signed_bytes(const mrc_header &header) noexcept;

/**
 * @brief Get the data type the values of a file are held in.
 *
 * @param header The header to consult.
 * @return numerical_type The data type, resolving mode 0 through
 * @ref holds_signed_bytes.
 */
numerical_type get_data_type(const mrc_header &header) noexcept;

/**
 * @brief Get where the values of a file begin.
 *
 * @param header The header to consult.
 * @return std::size_t The offset in bytes, past the main header and the
 * extended one.
 */
std::size_t get_data_offset(const mrc_header &header) noexcept;

} // namespace mrc
} // namespace em
} // namespace rexlib
