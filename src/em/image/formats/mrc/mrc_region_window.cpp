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

mrc_region_window make_region_window(
	const image_transfer_plan &regions,
	const mrc_geometry &geometry
) noexcept
{
	mrc_region_window result = {0, 0};

	const auto region_count = regions.get_region_count();
	if (region_count == 0)
	{
		return result;
	}

	const auto span_per_region =
		get_region_extent(regions, regions.get_file_rank(), 0);

	auto first = regions.get_file_offset(0)[0];
	auto last = first + span_per_region;
	for (std::size_t i = 1; i < region_count; ++i)
	{
		const auto position = regions.get_file_offset(i)[0];
		first = std::min(first, position);
		last = std::max(last, position + span_per_region);
	}

	const auto positions = geometry.get_extents()[0];
	first = std::min(first, positions);
	last = std::min(last, positions);

	const auto position_size =
		static_cast<std::size_t>(geometry.get_strides()[0]) *
		get_size(geometry.get_data_type());

	result.byte_offset = first * position_size;
	result.byte_size = (last - first) * position_size;

	return result;
}

} // namespace mrc
} // namespace em
} // namespace rexlib
