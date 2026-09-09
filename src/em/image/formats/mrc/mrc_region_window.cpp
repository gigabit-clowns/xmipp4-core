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

	const auto span_per_region =
		get_region_extent(regions, regions.get_file_rank(), 0);
	const auto positions = geometry.get_extents()[0];

	// A region's position is clamped to `positions` before it takes part in
	// any arithmetic to avoid underflow situations.
	const auto clamped = [positions] (std::size_t position) noexcept
	{
		return std::min(position, positions);
	};

	auto first = clamped(regions.get_file_offset(0)[0]);
	auto last = std::min(first + span_per_region, positions);
	for (std::size_t i = 1; i < region_count; ++i)
	{
		const auto position = clamped(regions.get_file_offset(i)[0]);
		first = std::min(first, position);
		last = std::max(last, std::min(position + span_per_region, positions));
	}

	const auto position_size =
		static_cast<std::size_t>(geometry.get_strides()[0]) *
		get_size(geometry.get_data_type());

	return mrc_region_window(
		first * position_size, (last - first) * position_size);
}

} // namespace mrc
} // namespace em
} // namespace rexlib
