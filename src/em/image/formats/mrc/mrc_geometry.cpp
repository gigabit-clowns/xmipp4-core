// SPDX-License-Identifier: GPL-3.0-only

#include "mrc_geometry.hpp"

#include "mrc_constants.hpp"

#include <rexlib/core/exceptions/invalid_operation_error.hpp>
#include <rexlib/em/image/exceptions/image_format_error.hpp>

#include <functional>
#include <numeric>
#include <stdexcept>

namespace rexlib
{
namespace em
{
namespace mrc
{

namespace
{

std::size_t to_extent(std::int32_t count) noexcept
{
	return static_cast<std::size_t>(count);
}

std::vector<std::size_t> derive_extents(const mrc_header &header)
{
	const auto columns = to_extent(header.get_column_count());
	const auto rows = to_extent(header.get_row_count());
	const auto sections = to_extent(header.get_section_count());

	if (is_volume_stack_space_group(header.get_space_group()))
	{
		const auto depth = to_extent(header.get_section_sampling());
		return {sections / depth, depth, rows, columns};
	}

	if (header.get_space_group() == image_stack_space_group && sections == 1)
	{
		return {rows, columns};
	}

	return {sections, rows, columns};
}

std::size_t derive_core_rank(const mrc_header &header) noexcept
{
	if (is_volume_stack_space_group(header.get_space_group()))
	{
		return 3;
	}

	return header.get_space_group() == image_stack_space_group ? 2 : 3;
}

std::vector<std::ptrdiff_t>
derive_strides(const std::vector<std::size_t> &extents)
{
	std::vector<std::ptrdiff_t> strides(extents.size());

	std::ptrdiff_t stride = 1;
	for (auto axis = extents.size(); axis > 0; --axis)
	{
		strides[axis - 1] = stride;
		stride *= static_cast<std::ptrdiff_t>(extents[axis - 1]);
	}

	return strides;
}

void check_element_alignment(std::size_t offset, numerical_type data_type)
{
	const auto element_size = get_size(data_type);
	if (element_size == 0 || offset % element_size != 0)
	{
		throw image_format_error(
			"mrc_geometry: The values of the file do not begin at an offset "
			"their data type can be addressed at."
		);
	}
}

} // anonymous namespace

mrc_geometry::mrc_geometry(const mrc_header &header)
	: m_extents(derive_extents(header))
	, m_strides(derive_strides(m_extents))
	, m_core_rank(derive_core_rank(header))
	, m_data_type(mrc::get_data_type(header))
	, m_data_offset(mrc::get_data_offset(header))
{
	check_element_alignment(m_data_offset, m_data_type);
}

span<const std::size_t> mrc_geometry::get_extents() const noexcept
{
	return make_span(m_extents.data(), m_extents.size());
}

std::size_t mrc_geometry::get_core_rank() const noexcept
{
	return m_core_rank;
}

span<const std::ptrdiff_t> mrc_geometry::get_strides() const noexcept
{
	return make_span(m_strides.data(), m_strides.size());
}

numerical_type mrc_geometry::get_data_type() const noexcept
{
	return m_data_type;
}

std::size_t mrc_geometry::get_data_offset() const noexcept
{
	return m_data_offset;
}

std::size_t mrc_geometry::get_element_count() const noexcept
{
	return std::accumulate(
		m_extents.cbegin(),
		m_extents.cend(),
		std::size_t(1),
		std::multiplies<std::size_t>()
	);
}

std::size_t mrc_geometry::get_data_size() const noexcept
{
	return get_element_count() * get_size(m_data_type);
}

mrc_header make_header(
	span<const std::size_t> extents,
	std::size_t core_rank,
	numerical_type data_type
)
{
	const auto rank = extents.size();
	if (core_rank == 0 || core_rank > rank)
	{
		throw std::invalid_argument(
			"mrc::make_header: The core rank must name at least one and at "
			"most every extent."
		);
	}

	if (rank < 2 || rank > 4)
	{
		throw invalid_operation_error(
			"mrc::make_header: The MRC format holds no file of that rank."
		);
	}

	mrc_header header;
	header.set_mode(get_mode(data_type));
	if (needs_imod_unsigned_flag(data_type))
	{
		header.set_imod_stamp(imod_stamp_value);
	}

	const auto columns = static_cast<std::int32_t>(extents[rank - 1]);
	const auto rows = static_cast<std::int32_t>(extents[rank - 2]);
	header.set_column_count(columns);
	header.set_row_count(rows);
	header.set_column_sampling(columns);
	header.set_row_sampling(rows);

	if (rank == 2 && core_rank == 2)
	{
		header.set_section_count(1);
		header.set_section_sampling(1);
		header.set_space_group(image_stack_space_group);
	}
	else if (rank == 3 && core_rank == 2)
	{
		header.set_section_count(static_cast<std::int32_t>(extents[0]));
		header.set_section_sampling(1);
		header.set_space_group(image_stack_space_group);
	}
	else if (rank == 3 && core_rank == 3)
	{
		const auto sections = static_cast<std::int32_t>(extents[0]);
		header.set_section_count(sections);
		header.set_section_sampling(sections);
		header.set_space_group(volume_space_group);
	}
	else if (rank == 4 && core_rank == 3)
	{
		const auto depth = static_cast<std::int32_t>(extents[1]);
		header.set_section_count(
			static_cast<std::int32_t>(extents[0] * extents[1]));
		header.set_section_sampling(depth);
		header.set_space_group(first_volume_stack_space_group);
	}
	else
	{
		throw invalid_operation_error(
			"mrc::make_header: The MRC format holds no file of that rank "
			"and core rank."
		);
	}

	header.set_cell_size({{
		static_cast<float>(header.get_column_count()),
		static_cast<float>(header.get_row_count()),
		static_cast<float>(header.get_section_sampling())
	}});
	header.set_cell_angles({{90.0F, 90.0F, 90.0F}});
	header.set_column_axis(1);
	header.set_row_axis(2);
	header.set_section_axis(3);
	header.set_version(written_version);

	// The sentinels the format reserves for statistics that were not
	// computed: a minimum above the maximum, a mean below both, and a
	// negative deviation.
	header.set_data_min(0.0F);
	header.set_data_max(-1.0F);
	header.set_data_mean(-2.0F);
	header.set_data_rms(-1.0F);

	return header;
}

} // namespace mrc
} // namespace em
} // namespace rexlib
