// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/constexpr.hpp>

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Random access iterator that bundles a pointer and a stride.
 *
 * Advancing the iterator moves the underlying pointer by `stride` elements
 * instead of a single one. This allows using strided memory regions with
 * standard algorithms such as `std::copy_n`, `std::fill_n` or
 * `std::transform`.
 *
 * @tparam T Element type. May be cv-qualified.
 * @tparam Stride Type representing the stride. Either an integer type or a
 * `std::integral_constant` specialization for a statically known stride.
 */
template <typename T, typename Stride>
class strided_pointer_iterator
{
public:
	using value_type = typename std::remove_cv<T>::type;
	using reference = T&;
	using pointer = T*;
	using difference_type = std::ptrdiff_t;
	using stride_type = Stride;
	using iterator_category = std::random_access_iterator_tag;

	/**
	 * @brief Construct a null iterator.
	 */
	XMIPP4_CONSTEXPR strided_pointer_iterator() noexcept;

	/**
	 * @brief Construct a new strided pointer iterator.
	 *
	 * @param data Pointer to the first element.
	 * @param stride Element step performed on each increment.
	 */
	XMIPP4_CONSTEXPR strided_pointer_iterator(
		pointer data,
		stride_type stride
	) noexcept;

	/**
	 * @brief Convert from a compatible strided pointer iterator.
	 *
	 * Enables the implicit conversion from a mutable iterator to a constant
	 * one.
	 *
	 * @tparam U Element type of the source iterator. Its pointer must be
	 * convertible to `pointer`.
	 * @tparam Stride2 Stride type of the source iterator. Must be convertible
	 * to `stride_type`.
	 * @param other The iterator to convert from.
	 */
	template <
		typename U,
		typename Stride2,
		typename = typename std::enable_if<
			std::is_convertible<U*, pointer>::value &&
			std::is_convertible<Stride2, stride_type>::value
		>::type
	>
	XMIPP4_CONSTEXPR strided_pointer_iterator(
		const strided_pointer_iterator<U, Stride2> &other
	) noexcept;

	strided_pointer_iterator(const strided_pointer_iterator &other) = default;
	strided_pointer_iterator(strided_pointer_iterator &&other) = default;
	~strided_pointer_iterator() = default;

	strided_pointer_iterator&
	operator=(const strided_pointer_iterator &other) = default;
	strided_pointer_iterator&
	operator=(strided_pointer_iterator &&other) = default;

	XMIPP4_CONSTEXPR reference operator*() const noexcept;
	XMIPP4_CONSTEXPR pointer operator->() const noexcept;
	XMIPP4_CONSTEXPR reference operator[](difference_type n) const noexcept;

	XMIPP4_CONSTEXPR strided_pointer_iterator& operator++() noexcept;
	XMIPP4_CONSTEXPR strided_pointer_iterator operator++(int) noexcept;
	XMIPP4_CONSTEXPR strided_pointer_iterator& operator--() noexcept;
	XMIPP4_CONSTEXPR strided_pointer_iterator operator--(int) noexcept;
	XMIPP4_CONSTEXPR strided_pointer_iterator&
	operator+=(difference_type n) noexcept;
	XMIPP4_CONSTEXPR strided_pointer_iterator&
	operator-=(difference_type n) noexcept;

	/**
	 * @brief Get a pointer to the current element.
	 *
	 * @return pointer Pointer to the current element.
	 */
	XMIPP4_CONSTEXPR pointer data() const noexcept;

	/**
	 * @brief Get the element step performed on each increment.
	 *
	 * @return stride_type The stride.
	 */
	XMIPP4_CONSTEXPR stride_type stride() const noexcept;

private:
	pointer m_data;
	stride_type m_stride;

};

template <typename T, typename Stride>
XMIPP4_CONSTEXPR strided_pointer_iterator<T, Stride> operator+(
	strided_pointer_iterator<T, Stride> it,
	std::ptrdiff_t n
) noexcept;

template <typename T, typename Stride>
XMIPP4_CONSTEXPR strided_pointer_iterator<T, Stride> operator+(
	std::ptrdiff_t n,
	strided_pointer_iterator<T, Stride> it
) noexcept;

template <typename T, typename Stride>
XMIPP4_CONSTEXPR strided_pointer_iterator<T, Stride> operator-(
	strided_pointer_iterator<T, Stride> it,
	std::ptrdiff_t n
) noexcept;

template <typename T, typename Stride>
XMIPP4_CONSTEXPR bool operator==(
	const strided_pointer_iterator<T, Stride> &lhs,
	const strided_pointer_iterator<T, Stride> &rhs
) noexcept;

template <typename T, typename Stride>
XMIPP4_CONSTEXPR bool operator!=(
	const strided_pointer_iterator<T, Stride> &lhs,
	const strided_pointer_iterator<T, Stride> &rhs
) noexcept;

/**
 * @brief Construct a `strided_pointer_iterator` deducing its element type.
 *
 * @tparam T Element type. May be cv-qualified.
 * @tparam Stride Stride type. May be an integer type or a specialization of
 * @ref std::integral_constant
 * @param data Pointer to the first element.
 * @param stride Element step performed on each increment.
 * @return strided_pointer_iterator<T, Stride> The newly created iterator.
 */
template <typename T, typename Stride>
XMIPP4_CONSTEXPR
strided_pointer_iterator<T, Stride> make_strided_pointer_iterator(
	T* data,
	Stride stride
) noexcept;

} // namespace multidimensional
} // namespace xmipp4

#include "strided_pointer_iterator.inl"
