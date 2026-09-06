// SPDX-License-Identifier: GPL-3.0-only

#include "mrc_region_transfer.hpp"

#include <rexlib/core/exceptions/invalid_operation_error.hpp>
#include <rexlib/core/layout/joint_layout_builder.hpp>
#include <rexlib/core/numerical/fixed_width_float.hpp>
#include <rexlib/em/image/image_transfer_plan.hpp>

#include <complex>
#include <stdexcept>

namespace rexlib
{
namespace em
{
namespace mrc
{

// What read_regions and write_regions are defined by lives in
// mrc_region_transfer_impl.hpp and is instantiated once per element type, in
// the mrc_region_transfer_<type>.cpp files. Only the geometry and the
// dispatch onto them are here.

namespace
{

span<const std::ptrdiff_t>
trailing(span<const std::ptrdiff_t> strides, std::size_t count)
{
	return make_span(strides.data() + (strides.size() - count), count);
}

void check_rank(
	std::size_t actual,
	std::size_t expected,
	const char *message
)
{
	if (actual != expected)
	{
		throw std::invalid_argument(message);
	}
}

// The extents of a plan cover the trailing axes of a side, which spans a
// single position along the leading ones, so the extent of an axis is
// resolved through get_region_extent rather than by indexing them directly.
std::ptrdiff_t resolve_offset(
	const image_transfer_plan &regions,
	span<const std::size_t> region_offset,
	span<const std::size_t> extents,
	span<const std::ptrdiff_t> strides
)
{
	const auto rank = extents.size();

	std::ptrdiff_t offset = 0;
	for (std::size_t axis = 0; axis < rank; ++axis)
	{
		const auto extent = get_region_extent(regions, rank, axis);
		if (region_offset[axis] + extent > extents[axis])
		{
			throw std::out_of_range(
				"mrc_region_transfer: A region does not fit where it is "
				"placed."
			);
		}

		offset += static_cast<std::ptrdiff_t>(region_offset[axis]) *
			strides[axis];
	}

	return offset;
}

} // anonymous namespace

mrc_region_transfer::mrc_region_transfer(
	const image_transfer_plan &regions,
	span<const std::size_t> file_extents,
	span<const std::ptrdiff_t> file_strides,
	span<const std::size_t> array_extents,
	span<const std::ptrdiff_t> array_strides,
	std::ptrdiff_t array_offset
)
{
	check_rank(
		file_extents.size(),
		regions.get_file_rank(),
		"mrc_region_transfer: The file extents do not have the file rank of "
		"the plan."
	);
	check_rank(
		array_extents.size(),
		regions.get_array_rank(),
		"mrc_region_transfer: The array extents do not have the array rank "
		"of the plan."
	);
	check_rank(
		file_strides.size(),
		file_extents.size(),
		"mrc_region_transfer: The file strides do not have the rank of its "
		"extents."
	);
	check_rank(
		array_strides.size(),
		array_extents.size(),
		"mrc_region_transfer: The array strides do not have the rank of its "
		"extents."
	);

	const auto count = regions.get_region_count();
	m_array_offsets.reserve(count);
	m_file_offsets.reserve(count);
	for (std::size_t i = 0; i < count; ++i)
	{
		m_file_offsets.push_back(
			resolve_offset(
				regions,
				regions.get_file_offset(i),
				file_extents,
				file_strides
			)
		);
		m_array_offsets.push_back(
			array_offset +
			resolve_offset(
				regions,
				regions.get_array_offset(i),
				array_extents,
				array_strides
			)
		);
	}

	const auto region_rank = regions.get_rank();
	joint_layout_builder builder;
	builder.set_extents(regions.get_extents());
	builder.add_operand(
		regions.get_extents(), trailing(array_strides, region_rank), 0);
	builder.add_operand(
		regions.get_extents(), trailing(file_strides, region_rank), 0);
	m_layout = builder.build();
}

mrc_region_transfer::mrc_region_transfer(
	mrc_region_transfer &&other
) noexcept = default;

mrc_region_transfer::~mrc_region_transfer() = default;

mrc_region_transfer& mrc_region_transfer::operator=(
	mrc_region_transfer &&other
) noexcept = default;

std::size_t mrc_region_transfer::get_region_count() const noexcept
{
	return m_array_offsets.size();
}

void mrc_region_transfer::read(
	void *array_data,
	numerical_type array_type,
	const byte *file_data,
	numerical_type file_type,
	byte_order file_order
) const
{
	const auto swapped = file_order != get_system_byte_order();

	switch (file_type)
	{
	#define REXLIB_MRC_READ_CASE(type, ...) \
		case numerical_type::type: \
			read_regions( \
				m_layout, \
				m_array_offsets, \
				m_file_offsets, \
				array_data, \
				array_type, \
				reinterpret_cast<const __VA_ARGS__*>(file_data), \
				swapped \
			); \
			break

	REXLIB_MRC_READ_CASE(int8, std::int8_t);
	REXLIB_MRC_READ_CASE(uint8, std::uint8_t);
	REXLIB_MRC_READ_CASE(int16, std::int16_t);
	REXLIB_MRC_READ_CASE(uint16, std::uint16_t);
	REXLIB_MRC_READ_CASE(float16, float16_t);
	REXLIB_MRC_READ_CASE(float32, float32_t);
	REXLIB_MRC_READ_CASE(complex_float32, std::complex<float32_t>);

	#undef REXLIB_MRC_READ_CASE

	default:
		throw invalid_operation_error(
			"mrc_region_transfer: No MRC file holds that data type."
		);
	}
}

void mrc_region_transfer::write(
	const void *array_data,
	numerical_type array_type,
	byte *file_data,
	numerical_type file_type,
	byte_order file_order
) const
{
	const auto swapped = file_order != get_system_byte_order();

	switch (file_type)
	{
	#define REXLIB_MRC_WRITE_CASE(type, ...) \
		case numerical_type::type: \
			write_regions( \
				m_layout, \
				m_array_offsets, \
				m_file_offsets, \
				array_data, \
				array_type, \
				reinterpret_cast<__VA_ARGS__*>(file_data), \
				swapped \
			); \
			break

	REXLIB_MRC_WRITE_CASE(int8, std::int8_t);
	REXLIB_MRC_WRITE_CASE(uint8, std::uint8_t);
	REXLIB_MRC_WRITE_CASE(int16, std::int16_t);
	REXLIB_MRC_WRITE_CASE(uint16, std::uint16_t);
	REXLIB_MRC_WRITE_CASE(float16, float16_t);
	REXLIB_MRC_WRITE_CASE(float32, float32_t);
	REXLIB_MRC_WRITE_CASE(complex_float32, std::complex<float32_t>);

	#undef REXLIB_MRC_WRITE_CASE

	default:
		throw invalid_operation_error(
			"mrc_region_transfer: No MRC file holds that data type."
		);
	}
}

} // namespace mrc
} // namespace em
} // namespace rexlib
