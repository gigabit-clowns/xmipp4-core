// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "mrc_region_transfer.hpp"

#include "mrc_transfer.hpp"

#include <backends/cpu/loops/elementwise_loop.hpp>
#include <rexlib/core/exceptions/invalid_operation_error.hpp>
#include <rexlib/core/numerical/numerical_type_dispatch.hpp>
#include <rexlib/core/platform/attributes.hpp>

#include <type_traits>

namespace rexlib
{
namespace em
{
namespace mrc
{
namespace detail
{

/**
 * @brief Whether a conversion between two element types exists.
 *
 * Convertibility is a property of the static types with no
 * @ref numerical_type counterpart, so it is asked of them directly, as the
 * CPU copy builder asks it.
 */
template <typename Destination, typename Source>
struct transfer_support : std::is_convertible<Source, Destination>
{
};

/**
 * @brief Walk every region of a batch with one layout.
 *
 * Each region is the same iteration space reached through a different pair of
 * pointers, so the layout is built once and only the two bases move. The loop
 * over the regions is here rather than inside the layout for exactly that
 * reason.
 */
template <typename Kernel, typename ArrayPointer, typename FilePointer>
void run_regions(
	const Kernel &kernel,
	const joint_layout &layout,
	const std::vector<std::ptrdiff_t> &array_offsets,
	const std::vector<std::ptrdiff_t> &file_offsets,
	ArrayPointer array_data,
	FilePointer file_data
)
{
	for (std::size_t i = 0; i < array_offsets.size(); ++i)
	{
		cpu::run_elementwise_loop(
			kernel,
			layout,
			array_data + array_offsets[i],
			file_data + file_offsets[i]
		);
	}
}

template <typename Kernel, typename ArrayPointer, typename FilePointer>
void run_supported_regions(
	std::true_type,
	const Kernel &kernel,
	const joint_layout &layout,
	const std::vector<std::ptrdiff_t> &array_offsets,
	const std::vector<std::ptrdiff_t> &file_offsets,
	ArrayPointer array_data,
	FilePointer file_data
)
{
	run_regions(
		kernel, layout, array_offsets, file_offsets, array_data, file_data
	);
}

// The unsupported overload never instantiates a loop, which is what keeps one
// from being compiled for every pair of element types no conversion joins.
template <typename Kernel, typename ArrayPointer, typename FilePointer>
REXLIB_NORETURN
void run_supported_regions(
	std::false_type,
	const Kernel &,
	const joint_layout &,
	const std::vector<std::ptrdiff_t> &,
	const std::vector<std::ptrdiff_t> &,
	ArrayPointer,
	FilePointer
)
{
	throw invalid_operation_error(
		"mrc_region_transfer: The values of the file can not be converted "
		"into the data type asked for."
	);
}

} // namespace detail

template <typename Q>
void read_regions(
	const joint_layout &layout,
	const std::vector<std::ptrdiff_t> &array_offsets,
	const std::vector<std::ptrdiff_t> &file_offsets,
	void *array_data,
	numerical_type array_type,
	const Q *file_data,
	bool swapped
)
{
	dispatch_numerical_types(
		[&] (auto array_tag)
		{
			using T = typename decltype(array_tag)::type;
			const auto support = detail::transfer_support<T, Q>();
			auto *array = static_cast<T*>(array_data);

			if (swapped)
			{
				detail::run_supported_regions(
					support,
					mrc_byte_swapped_read_kernel(),
					layout,
					array_offsets,
					file_offsets,
					array,
					file_data
				);
			}
			else
			{
				detail::run_supported_regions(
					support,
					mrc_read_kernel(),
					layout,
					array_offsets,
					file_offsets,
					array,
					file_data
				);
			}
		},
		array_type
	);
}

template <typename Q>
void write_regions(
	const joint_layout &layout,
	const std::vector<std::ptrdiff_t> &array_offsets,
	const std::vector<std::ptrdiff_t> &file_offsets,
	const void *array_data,
	numerical_type array_type,
	Q *file_data,
	bool swapped
)
{
	dispatch_numerical_types(
		[&] (auto array_tag)
		{
			using T = typename decltype(array_tag)::type;
			const auto support = detail::transfer_support<Q, T>();
			const auto *array = static_cast<const T*>(array_data);

			if (swapped)
			{
				detail::run_supported_regions(
					support,
					mrc_byte_swapped_write_kernel(),
					layout,
					array_offsets,
					file_offsets,
					array,
					file_data
				);
			}
			else
			{
				detail::run_supported_regions(
					support,
					mrc_write_kernel(),
					layout,
					array_offsets,
					file_offsets,
					array,
					file_data
				);
			}
		},
		array_type
	);
}

/**
 * @brief Instantiate the region transfer for one element type of a file.
 *
 * Write it once in a translation unit of its own per element type. The array
 * side is a grid over every data type there is, each cell specialized on the
 * strides of both operands, so one element type already costs about what the
 * CPU copy builder costs.
 */
#define REXLIB_INSTANTIATE_MRC_REGION_TRANSFER(...) \
	template void read_regions<__VA_ARGS__>( \
		const joint_layout&, \
		const std::vector<std::ptrdiff_t>&, \
		const std::vector<std::ptrdiff_t>&, \
		void*, \
		numerical_type, \
		const __VA_ARGS__*, \
		bool \
	); \
	template void write_regions<__VA_ARGS__>( \
		const joint_layout&, \
		const std::vector<std::ptrdiff_t>&, \
		const std::vector<std::ptrdiff_t>&, \
		const void*, \
		numerical_type, \
		__VA_ARGS__*, \
		bool \
	)

} // namespace mrc
} // namespace em
} // namespace rexlib
