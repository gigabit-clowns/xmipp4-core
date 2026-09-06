// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <algorithm>
#include <complex>
#include <cstddef>
#include <type_traits>

namespace rexlib
{
namespace em
{
namespace mrc
{

/**
 * @brief Reverse the bytes of one element.
 *
 * Works on the bytes of the value rather than on a number read out of them,
 * which is what reverses a type that is not an integer itself, such as a
 * floating point value or one of the storage-only types. A compiler turns it
 * into the same byte swapping instruction either way.
 *
 * @tparam T The element type.
 * @param value The element to reverse.
 * @return T The element with its bytes reversed.
 */
template <typename T>
T byte_swap(const T &value) noexcept
{
	static_assert(
		std::is_trivially_copyable<T>::value,
		"Only the bytes of a trivially copyable value can be reversed."
	);

	T result = value;
	auto *bytes = reinterpret_cast<unsigned char*>(&result);
	std::reverse(bytes, bytes + sizeof(T));
	return result;
}

/**
 * @brief Reverse the bytes of each component of one complex element.
 *
 * A complex value is two numbers rather than one twice as wide, so its
 * components are reversed one at a time. Reversing all of its bytes at once
 * would exchange them as well.
 *
 * @tparam T The component type.
 * @param value The element to reverse.
 * @return std::complex<T> The element with the bytes of each component
 * reversed.
 */
template <typename T>
std::complex<T> byte_swap(const std::complex<T> &value) noexcept
{
	return std::complex<T>(
		byte_swap(value.real()),
		byte_swap(value.imag())
	);
}

} // namespace mrc
} // namespace em
} // namespace rexlib
