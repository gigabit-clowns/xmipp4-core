// SPDX-License-Identifier: GPL-3.0-only

#include "mrc_geometry.hpp"

#include "mrc_constants.hpp"

#include <rexlib/em/image/exceptions/image_format_error.hpp>

#include <functional>
#include <numeric>

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

} // namespace mrc
} // namespace em
} // namespace rexlib
