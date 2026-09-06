// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "mrc_file_mapping.hpp"
#include "mrc_geometry.hpp"
#include "mrc_header.hpp"

#include <rexlib/em/image/image_writer.hpp>

#include <string>

namespace rexlib
{
namespace em
{
namespace mrc
{

/**
 * @brief One MRC file created and opened for writing.
 *
 * The extents are complete when a writer is opened, so the file is laid out
 * in full there and then, header written and values sized: a region can be
 * written wherever it belongs and in any order afterwards, and nothing about
 * the file changes as it fills.
 *
 * A region that is never written keeps whatever the file was laid out with,
 * which is unspecified.
 */
class mrc_writer final
	: public image_writer
{
public:
	/**
	 * @brief Create a file and open it for writing.
	 *
	 * Any file already at that path is replaced.
	 *
	 * @param path Path to the file to create.
	 * @param extents Extents of the file, slowest axis first.
	 * @param core_rank How many trailing extents are one image or volume.
	 * @param data_type Data type of its elements.
	 * @throws std::invalid_argument If @p core_rank is zero or exceeds the
	 * rank of @p extents.
	 * @throws invalid_operation_error If the MRC format holds no file of
	 * that shape, or no mode holds @p data_type.
	 * @throws image_format_error If the file could not be created, sized or
	 * mapped.
	 */
	mrc_writer(
		const std::string &path,
		span<const std::size_t> extents,
		std::size_t core_rank,
		numerical_type data_type
	);

	~mrc_writer() override;

	span<const std::size_t> get_extents() const noexcept override;

	std::size_t get_core_rank() const noexcept override;

	numerical_type get_data_type() const noexcept override;

	void write(
		const_array_ref source,
		const image_transfer_plan &regions
	) override;

	void flush() override;

private:
	mrc_header m_header;
	mrc_geometry m_geometry;
	mrc_file_mapping m_mapping;
};

} // namespace mrc
} // namespace em
} // namespace rexlib
