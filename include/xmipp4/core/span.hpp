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
 * Implementation based on:
 * https://github.com/tcbrindle/span
 * 
 */

#include "platform/attributes.hpp"
#include "platform/constexpr.hpp"

#include <type_traits>
#include <limits>

namespace xmipp4 
{

/**
 * @brief Constant representing dynamic size of an array.
 * 
 */
XMIPP4_CONSTEXPR auto dynamic_extent = std::numeric_limits<std::size_t>::max(); 

namespace detail
{

/**
 * @brief Storage for the span object.
 * 
 * @tparam T Element type
 * @tparam S Static size
 */
template <typename T, std::size_t S>
struct span_storage 
{
    T* data;
    static XMIPP4_CONST_CONSTEXPR std::size_t size = S;

    span_storage() = delete;
    XMIPP4_CONSTEXPR span_storage(T* ptr, std::size_t count) noexcept;
    span_storage(const span_storage &other) = default;
    span_storage(span_storage && other) = default;
    ~span_storage() = default;

    span_storage& operator=(const span_storage &other) = default;
    span_storage& operator=(span_storage && other) = default;

};

/**
 * @brief Storage for the span object with dynamic sizing.
 * 
 * @tparam T 
 */
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

template <typename T>
struct span_storage<T, 0>
{
    static XMIPP4_CONST_CONSTEXPR T* data = nullptr;
    static XMIPP4_CONST_CONSTEXPR std::size_t size = 0;

    span_storage() = default;
    XMIPP4_CONSTEXPR span_storage(T* ptr, std::size_t count) noexcept;
    span_storage(const span_storage &other) = default;
    span_storage(span_storage && other) = default;
    ~span_storage() = default;

    span_storage& operator=(const span_storage &other) = default;
    span_storage& operator=(span_storage && other) = default;

};

} // namespace detail



/**
 * @brief Represents a non-owning area of contiguous memory.
 * 
 * Provides a pre C++20 implementation of std::span.
 * 
 * @tparam T Type of the elements.
 * @tparam Extent Size of the area. Use dynamic_extent dynamic assignation.
 */
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


    /**
     * @brief Construct from a pointer and size.
     * 
     * @param ptr Pointer to the data.
     * @param count Number of elements.
     */
    XMIPP4_CONSTEXPR span(pointer ptr, size_type count);

    /**
     * @brief Construct from a pointer pair.
     * 
     * @param first Pointer to the first element.
     * @param last Pointer to the past-the-end element.
     */
    XMIPP4_CONSTEXPR span(pointer first, pointer last);

    span() = default;
    span(const span &other) = default;
    span(span &&other) = default;
    ~span() = default;

    span& operator=(const span &other) = default;
    span& operator=(span &&other) = default;

    /**
     * @brief Get the number of referenced objects.
     * 
     * @return size_type Number of objects.
     */
    XMIPP4_CONSTEXPR size_type size() const noexcept;

    /**
     * @brief Get the size in bytes of the referenced objects.
     * 
     * @return size_type Number of bytes.
     */
    XMIPP4_CONSTEXPR size_type size_bytes() const noexcept;

    /**
     * @brief Check if no objects are references.
     * 
     * @return true If 0 objects are referenced.
     * @return false If 1 or more objects are referenced.
     */
    XMIPP4_NODISCARD XMIPP4_CONSTEXPR bool empty() const noexcept;

    /**
     * @brief Obtain a reference to an object by its index.
     * 
     * @param idx The index of the object. Must be in [0...size())
     * @return reference The reference to the requested object.
     * @note No boundary checks are performed. Using an index that
     * is greater or equal to the size leads to undefined behavior.
     */
    XMIPP4_CONSTEXPR reference operator[](size_type idx) const noexcept;

    /**
     * @brief Obtain a reference to the first object.
     * 
     * @return reference Reference to the first object.
     * @note No emptiness check is performed. Calling this method
     * when empty() == true leads to undefined behavior.
     */
    XMIPP4_CONSTEXPR reference front() const noexcept;

    /**
     * @brief Obtain a reference to the last object.
     * 
     * @return reference Reference to the last object.
     * @note No emptiness check is performed. Calling this method
     * when empty() == true leads to undefined behavior.
     */
    XMIPP4_CONSTEXPR reference back() const noexcept;

    /**
     * @brief Get a pointer to the referenced data.
     * 
     * @return pointer Pointer to the referenced data.
     */
    XMIPP4_CONSTEXPR pointer data() const noexcept;

    /**
     * @brief Get an iterator to the first item.
     * 
     * @return iterator Iterator to the first item. 
     */
    XMIPP4_CONSTEXPR iterator begin() const noexcept;

    /**
     * @brief Get an iterator to the past-the-end item.
     * 
     * @return iterator Iterator to the past the end item.
     */
    XMIPP4_CONSTEXPR iterator end() const noexcept;

    /**
     * @brief Get a reverse iterator to the last item.
     * 
     * @return reverse_iterator Reverse iterator to the last item. 
     */
    XMIPP4_CONSTEXPR reverse_iterator rbegin() const noexcept;

    /**
     * @brief Get a reverse iterator to the before-first item.
     * 
     * @return reverse_iterator Reverse iterator to the before-first item. 
     */
    XMIPP4_CONSTEXPR reverse_iterator rend() const noexcept;

private:
    storage_type m_storage;

};

} // namespace xmipp4

#include "span.inl"
