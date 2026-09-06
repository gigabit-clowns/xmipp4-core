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
 * @brief Move one element between a file and an array.
 *
 * The file is the second operand in both directions, which is what lets one
 * pair of overloads serve a read and a write: reading writes through the
 * first and takes the second as const, writing does the opposite, and no
 * other combination is viable, so the direction is settled by the
 * qualification of the pointers alone.
 *
 * Conversion goes through @ref cpu::cast rather than @ref numerical_cast,
 * which reaches @ref float16_t through a constructor exported across the
 * shared object boundary and so cannot be vectorized.
 */
struct mrc_transfer
{
	/**
	 * @brief Read one element of the file into the array.
	 *
	 * @param array Where the value is written.
	 * @param file Where it is read from.
	 */
	template <typename T, typename Q>
	void operator()(T *array, const Q *file) const noexcept
	{
		cpu::cast(array, file);
	}

	/**
	 * @brief Write one element of the array into the file.
	 *
	 * @param array Where the value is read from.
	 * @param file Where it is written.
	 */
	template <typename T, typename Q>
	void operator()(const T *array, Q *file) const noexcept
	{
		cpu::cast(file, array);
	}
};

/**
 * @brief Move one element between a file of the other byte order and an
 * array.
 *
 * The bytes are reversed on the side that is the file, in the type the file
 * holds them in: a read reverses before converting and a write converts
 * before reversing. Doing it the other way around would reverse a value of
 * the array's type, which is a different number whenever the two types differ
 * in width.
 *
 * @see mrc_transfer
 */
struct mrc_transfer_byte_swapped
{
	/**
	 * @brief Read one element of the file into the array.
	 *
	 * @param array Where the value is written.
	 * @param file Where it is read from.
	 */
	template <typename T, typename Q>
	void operator()(T *array, const Q *file) const noexcept
	{
		const auto value = byte_swap(*file);
		cpu::cast(array, &value);
	}

	/**
	 * @brief Write one element of the array into the file.
	 *
	 * @param array Where the value is read from.
	 * @param file Where it is written.
	 */
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
