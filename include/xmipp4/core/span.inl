/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file span.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of span.hpp
 * @date 2024-10-24
 * 
 */

#include "span.hpp"

#include "platform/assert.hpp"

#include <unordered_map>
#include <type_traits>

namespace xmipp4 
{

namespace detail
{

template <typename T, std::size_t S>
XMIPP4_INLINE_CONSTEXPR 
span_storage<T, S>::span_storage() noexcept
    : data(nullptr)
{
}

template <typename T, std::size_t S>
XMIPP4_INLINE_CONSTEXPR 
span_storage<T, S>::span_storage(T* ptr, std::size_t) noexcept
    : data(ptr)
{
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
span_storage<T, dynamic_extent>::span_storage() noexcept
    : data(nullptr)
    , size(0)
{
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
span_storage<T, dynamic_extent>::span_storage(T* ptr, std::size_t count) noexcept
    : data(ptr)
    , size(count)
{
}

} // namespace detail





template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR span<T, Extent>::span(pointer ptr, size_type count)
    : m_storage(ptr, count)
{
}

template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR span<T, Extent>::span(pointer first, pointer last)
    : span(first, std::distance(first, last))
{
}


template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR 
typename span<T, Extent>::size_type span<T, Extent>::size() const noexcept
{
    return m_storage.size;
}

template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR 
typename span<T, Extent>::size_type span<T, Extent>::size_bytes() const noexcept
{
    return size() * sizeof(element_type);
}

template <typename T, std::size_t Extent>
XMIPP4_NODISCARD XMIPP4_INLINE_CONSTEXPR 
bool span<T, Extent>::empty() const noexcept
{
    return size() == 0;
}

template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR 
typename span<T, Extent>::reference 
span<T, Extent>::operator[](size_type idx) const noexcept
{
    return data()[idx];
}

template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR 
typename span<T, Extent>::reference span<T, Extent>::front() const noexcept
{
    return *(data());
}

template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR 
typename span<T, Extent>::reference span<T, Extent>::back() const noexcept
{
    return *(data() + size() - 1);
}

template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR 
typename span<T, Extent>::pointer span<T, Extent>::data() const noexcept
{
    return m_storage.data;
}

template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR 
typename span<T, Extent>::iterator span<T, Extent>::begin() const noexcept
{
    return data();
}

template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR 
typename span<T, Extent>::iterator span<T, Extent>::end() const noexcept
{
    return data() + size();
}

template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR 
typename span<T, Extent>::reverse_iterator 
span<T, Extent>::rbegin() const noexcept
{
    return reverse_iterator(end());
}

template <typename T, std::size_t Extent>
XMIPP4_INLINE_CONSTEXPR 
typename span<T, Extent>::reverse_iterator 
span<T, Extent>::rend() const noexcept
{
    return reverse_iterator(begin());
}

} // namespace xmipp4
