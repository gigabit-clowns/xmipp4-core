#pragma once

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
 * @file span.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides span enumeration and utility functions
 * @date 2024-10-24
 * 
 */

#include "platform/attributes.hpp"
#include "platform/constexpr.hpp"

#include <limits>

namespace xmipp4 
{

XMIPP4_CONSTEXPR auto dynamic_extent = std::numeric_limits<std::size_t>::max(); 

namespace detail
{

template <typename T, std::size_t S>
struct span_storage 
{
    T* data;
    static XMIPP4_CONST_CONSTEXPR std::size_t size = S;

    XMIPP4_CONSTEXPR span_storage() noexcept;
    XMIPP4_CONSTEXPR span_storage(T* ptr, std::size_t count) noexcept;
    span_storage(const span_storage &other) = default;
    span_storage(span_storage && other) = default;
    ~span_storage() = default;

    span_storage& operator=(const span_storage &other) = default;
    span_storage& operator=(span_storage && other) = default;

};

template <typename T>
struct span_storage<T, dynamic_extent> 
{
    T* data;
    std::size_t size;

    XMIPP4_CONSTEXPR span_storage() noexcept;
    XMIPP4_CONSTEXPR span_storage(T* ptr, std::size_t count) noexcept;
    span_storage(const span_storage &other) = default;
    span_storage(span_storage && other) = default;
    ~span_storage() = default;

    span_storage& operator=(const span_storage &other) = default;
    span_storage& operator=(span_storage && other) = default;
};

} // namespace detail


template <typename T, std::size_t Extent = dynamic_extent>
class span 
{
    using storage_type = detail::span_storage<T, Extent>;

public:
    using element_type = T;
    using value_type = typename std::remove_cv<element_type>::type;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = element_type*;
    using const_pointer = const element_type*;
    using reference = element_type&;
    using const_reference = const element_type&;
    using iterator = pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;

    static constexpr size_type extent = Extent;

    XMIPP4_CONSTEXPR span(pointer ptr, size_type count);
    XMIPP4_CONSTEXPR span(pointer first, pointer last);
    span(const span &other) = default;
    span(span &&other) = default;
    ~span() = default;

    span& operator=(const span &other) = default;
    span& operator=(span &&other) = default;


    XMIPP4_CONSTEXPR size_type size() const noexcept;

    XMIPP4_CONSTEXPR size_type size_bytes() const noexcept;

    XMIPP4_NODISCARD XMIPP4_CONSTEXPR bool empty() const noexcept;

    XMIPP4_CONSTEXPR reference operator[](size_type idx) const noexcept;

    XMIPP4_CONSTEXPR reference front() const noexcept;

    XMIPP4_CONSTEXPR reference back() const noexcept;

    XMIPP4_CONSTEXPR pointer data() const noexcept;

    XMIPP4_CONSTEXPR iterator begin() const noexcept;

    XMIPP4_CONSTEXPR iterator end() const noexcept;

    XMIPP4_CONSTEXPR reverse_iterator rbegin() const noexcept;

    XMIPP4_CONSTEXPR reverse_iterator rend() const noexcept;

private:
    storage_type m_storage;

};

} // namespace xmipp4

#include "span.inl"
