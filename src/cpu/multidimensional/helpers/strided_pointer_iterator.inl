// SPDX-License-Identifier: GPL-3.0-only

#include "strided_pointer_iterator.hpp"

namespace xmipp4
{
namespace multidimensional
{

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride>::strided_pointer_iterator() noexcept
	: m_data(nullptr)
	, m_stride()
{
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride>::strided_pointer_iterator(
	pointer data,
	stride_type stride
) noexcept
	: m_data(data)
	, m_stride(stride)
{
}

template <typename T, typename Stride>
template <typename U, typename Stride2, typename>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride>::strided_pointer_iterator(
	const strided_pointer_iterator<U, Stride2> &other
) noexcept
	: m_data(other.data())
	, m_stride(other.stride())
{
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
typename strided_pointer_iterator<T, Stride>::reference
strided_pointer_iterator<T, Stride>::operator*() const noexcept
{
	return *m_data;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
typename strided_pointer_iterator<T, Stride>::pointer
strided_pointer_iterator<T, Stride>::operator->() const noexcept
{
	return m_data;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
typename strided_pointer_iterator<T, Stride>::reference
strided_pointer_iterator<T, Stride>::operator[](difference_type n) const noexcept
{
	return m_data[n*m_stride];
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride>&
strided_pointer_iterator<T, Stride>::operator++() noexcept
{
	m_data += m_stride;
	return *this;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride>
strided_pointer_iterator<T, Stride>::operator++(int) noexcept
{
	const strided_pointer_iterator result(*this);
	++(*this);
	return result;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride>&
strided_pointer_iterator<T, Stride>::operator--() noexcept
{
	m_data -= m_stride;
	return *this;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride>
strided_pointer_iterator<T, Stride>::operator--(int) noexcept
{
	const strided_pointer_iterator result(*this);
	--(*this);
	return result;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride>&
strided_pointer_iterator<T, Stride>::operator+=(difference_type n) noexcept
{
	m_data += n*m_stride;
	return *this;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride>&
strided_pointer_iterator<T, Stride>::operator-=(difference_type n) noexcept
{
	m_data -= n*m_stride;
	return *this;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
typename strided_pointer_iterator<T, Stride>::pointer
strided_pointer_iterator<T, Stride>::data() const noexcept
{
	return m_data;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
typename strided_pointer_iterator<T, Stride>::stride_type
strided_pointer_iterator<T, Stride>::stride() const noexcept
{
	return m_stride;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride> operator+(
	strided_pointer_iterator<T, Stride> it,
	std::ptrdiff_t n
) noexcept
{
	it += n;
	return it;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride> operator+(
	std::ptrdiff_t n,
	strided_pointer_iterator<T, Stride> it
) noexcept
{
	it += n;
	return it;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride> operator-(
	strided_pointer_iterator<T, Stride> it,
	std::ptrdiff_t n
) noexcept
{
	it -= n;
	return it;
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
bool operator==(
	const strided_pointer_iterator<T, Stride> &lhs,
	const strided_pointer_iterator<T, Stride> &rhs
) noexcept
{
	return lhs.data() == rhs.data();
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
bool operator!=(
	const strided_pointer_iterator<T, Stride> &lhs,
	const strided_pointer_iterator<T, Stride> &rhs
) noexcept
{
	return !(lhs == rhs);
}

template <typename T, typename Stride>
XMIPP4_INLINE_CONSTEXPR
strided_pointer_iterator<T, Stride> make_strided_pointer_iterator(
	T* data,
	Stride stride
) noexcept
{
	return strided_pointer_iterator<T, Stride>(data, stride);
}

} // namespace multidimensional
} // namespace xmipp4
