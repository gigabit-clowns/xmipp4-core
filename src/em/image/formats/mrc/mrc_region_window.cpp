// SPDX-License-Identifier: GPL-3.0-only

#include "mrc_region_window.hpp"

#include "mrc_geometry.hpp"

#include <rexlib/core/numerical/numerical_type.hpp>
#include <rexlib/em/image/image_transfer_plan.hpp>

#include <algorithm>

namespace rexlib
{
namespace em
{
namespace mrc
{

mrc_region_window::mrc_region_window(
	std::size_t byte_offset,
	std::size_t byte_size
) noexcept
	: m_byte_offset(byte_offset)
	, m_byte_size(byte_size)
{
}

std::size_t mrc_region_window::get_byte_offset() const noexcept
{
	return m_byte_offset;
}

std::size_t mrc_region_window::get_byte_size() const noexcept
{
	return m_byte_size;
}

mrc_region_window make_region_window(
	const image_transfer_plan &regions,
	const mrc_geometry &geometry
) noexcept
{
	const auto region_count = regions.get_region_count();
	if (region_count == 0)
	{
		return mrc_region_window(0, 0);
	}

	// TODO properly derive this when axis ordering is accounted in mrc_geometry
	REXLIB_CONST_CONSTEXPR std::size_t slowest_axis = 0;

	const auto region_extent =
		get_region_extent(regions, regions.get_file_rank(), slowest_axis);

	auto first_position = regions.get_file_offset(0)[slowest_axis];
	REXLIB_ASSERT(first_position <= geometry.get_extents()[slowest_axis]);
	REXLIB_ASSERT(
		region_extent <= geometry.get_extents()[slowest_axis] - first_position
	);

	auto last_position = first_position + region_extent;
	for (std::size_t i = 1; i < region_count; ++i)
	{
		const auto position = regions.get_file_offset(i)[slowest_axis];
		REXLIB_ASSERT(
			position <= geometry.get_extents()[slowest_axis]
		);
		REXLIB_ASSERT(
			region_extent <= geometry.get_extents()[slowest_axis] - position
		);

		first_position = std::min(first_position, position);
		last_position = std::max(last_position, position + region_extent);
	}

	const auto stride_in_bytes =
		static_cast<std::size_t>(geometry.get_strides()[slowest_axis]) *
		get_size(geometry.get_data_type());

	return mrc_region_window(
		first_position * stride_in_bytes,
		(last_position - first_position) * stride_in_bytes
	);
}

} // namespace mrc
} // namespace em
} // namespace rexlib
