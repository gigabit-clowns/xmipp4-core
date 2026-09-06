// SPDX-License-Identifier: GPL-3.0-only

#include "mrc_writer.hpp"

#include "mrc_host_access.hpp"
#include "mrc_region_transfer.hpp"

#include <rexlib/core/ndarray/array_descriptor.hpp>
#include <rexlib/core/ndarray/const_array_ref.hpp>
#include <rexlib/em/image/image_transfer_plan.hpp>

#include <vector>

namespace rexlib
{
namespace em
{
namespace mrc
{

namespace
{

// The file is laid out before it is mapped, since a mapping can neither
// create one nor resize it.
mrc_file_mapping lay_out_file(
	const std::string &path,
	const mrc_geometry &geometry
)
{
	create_file(
		path,
		geometry.get_data_offset() + geometry.get_data_size()
	);

	return mrc_file_mapping(path, read_write);
}

} // anonymous namespace

mrc_writer::mrc_writer(
	const std::string &path,
	span<const std::size_t> extents,
	std::size_t core_rank,
	numerical_type data_type
)
	: m_header(make_header(extents, core_rank, data_type))
	, m_geometry(m_header)
	, m_mapping(lay_out_file(path, m_geometry))
{
	serialize_header(
		m_header,
		make_span(m_mapping.get_data(), m_mapping.get_size())
	);
}

mrc_writer::~mrc_writer()
{
	// A destructor can not report a failure, so a writer whose failures
	// matter is flushed explicitly beforehand. This is what keeps a writer
	// that was not from leaving the header unwritten.
	try
	{
		m_mapping.flush();
	}
	catch (const std::exception &)
	{
	}
}

span<const std::size_t> mrc_writer::get_extents() const noexcept
{
	return m_geometry.get_extents();
}

std::size_t mrc_writer::get_core_rank() const noexcept
{
	return m_geometry.get_core_rank();
}

numerical_type mrc_writer::get_data_type() const noexcept
{
	return m_geometry.get_data_type();
}

void mrc_writer::write(
	const_array_ref source,
	const image_transfer_plan &regions
)
{
	const auto *array_data = get_host_data(source);

	const auto &descriptor = source.get_descriptor();
	const auto &layout = descriptor.get_layout();

	std::vector<std::size_t> array_extents;
	std::vector<std::ptrdiff_t> array_strides;
	layout.get_extents(array_extents);
	layout.get_strides(array_strides);

	const mrc_region_transfer transfer(
		mrc_transfer_direction::write,
		regions,
		m_geometry.get_extents(),
		m_geometry.get_strides(),
		make_span(array_extents),
		make_span(array_strides),
		layout.get_offset()
	);

	transfer.write(
		array_data,
		descriptor.get_data_type(),
		m_mapping.get_data() + m_geometry.get_data_offset(),
		m_geometry.get_data_type(),
		m_header.get_byte_order()
	);
}

void mrc_writer::flush()
{
	m_mapping.flush();
}

} // namespace mrc
} // namespace em
} // namespace rexlib
