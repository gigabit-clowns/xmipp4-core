// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <rexlib/core/memory/byte_order.hpp>

#include <complex>
#include <cstdint>
#include <cstring>

namespace rexlib
{
namespace em
{
namespace mrc
{

namespace detail
{

template <std::size_t Size>
struct unsigned_of_size;

template <>
struct unsigned_of_size<1>
{
	using type = std::uint8_t;
};

template <>
struct unsigned_of_size<2>
{
	using type = std::uint16_t;
};

template <>
struct unsigned_of_size<4>
{
	using type = std::uint32_t;
};

template <>
struct unsigned_of_size<8>
{
	using type = std::uint64_t;
};

} // namespace detail

/**
 * @brief Reverse the bytes of one element.
 *
 * Reads the element as the unsigned integer of its width, which is what
 * reverses the bytes of a type that is not an integer itself, such as a
 * floating point value.
 *
 * @tparam T The element type.
 * @param value The element to reverse.
 * @return T The element with its bytes reversed.
 */
template <typename T>
T byte_swap(const T &value) noexcept
{
	using raw_type = typename detail::unsigned_of_size<sizeof(T)>::type;

	raw_type raw;
	std::memcpy(&raw, &value, sizeof(raw));
	raw = reverse_byte_order(raw);

	T result;
	std::memcpy(&result, &raw, sizeof(result));
	return result;
}

/**
 * @brief Reverse the bytes of each component of one complex element.
 *
 * A complex value is two numbers rather than one twice as wide, so its
 * components are reversed one at a time.
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
