// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "mrc_byte_swap.hpp"

#include <backends/cpu/load_store.hpp>

namespace rexlib
{
namespace em
{
namespace mrc
{

/**
 * @brief Read one element of a file into an array.
 *
 * Conversion goes through @ref cpu::cast rather than @ref numerical_cast:
 * the two implement the same cases, but numerical_cast reaches
 * @ref float16_t through constructors exported across the shared object
 * boundary, which is a call per element that no vectorizer gets through.
 */
struct mrc_read_kernel
{
	template <typename T, typename Q>
	void operator()(T *array, const Q *file) const noexcept
	{
		cpu::cast(array, file);
	}
};

/**
 * @brief Read one element of a file of the other byte order into an array.
 *
 * The bytes are reversed in the type the file holds them in, before the
 * conversion. Reversing a value of the array's type instead would be a
 * different number whenever the two widths differ.
 */
struct mrc_byte_swapped_read_kernel
{
	template <typename T, typename Q>
	void operator()(T *array, const Q *file) const noexcept
	{
		const auto value = byte_swap(*file);
		cpu::cast(array, &value);
	}
};

/**
 * @brief Write one element of an array into a file.
 *
 * @see mrc_read_kernel
 */
struct mrc_write_kernel
{
	template <typename T, typename Q>
	void operator()(const T *array, Q *file) const noexcept
	{
		cpu::cast(file, array);
	}
};

/**
 * @brief Write one element of an array into a file of the other byte order.
 *
 * The conversion happens first and the bytes are reversed afterwards, so that
 * they are reversed in the type the file holds them in.
 *
 * @see mrc_byte_swapped_read_kernel
 */
struct mrc_byte_swapped_write_kernel
{
	template <typename T, typename Q>
	void operator()(const T *array, Q *file) const noexcept
	{
		Q value;
		cpu::cast(&value, array);
		*file = byte_swap(value);
	}
};

} // namespace mrc
} // namespace em
} // namespace rexlib
