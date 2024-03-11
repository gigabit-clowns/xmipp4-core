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
 * @file pimpl.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides pimpl class
 * @date 2024-03-10
 * 
 */

#include "../platform/constexpr.hpp"
#include "../platform/attributes.hpp"

#include <memory>

namespace xmipp4 
{
namespace memory
{

struct defer_construct_tag {};
XMIPP4_INLINE_CONST_CONSTEXPR defer_construct_tag defer_construct;

template <typename T, typename Alloc = std::allocator<T>>
class pimpl
{
public:
    using value_type = T;
    using allocator_type = Alloc;
    using reference = typename allocator_type::reference;
    using const_reference = typename allocator_type::const_reference;
    using pointer = typename allocator_type::pointer;
    using const_pointer = typename allocator_type::const_pointer;

    explicit pimpl(defer_construct_tag) noexcept;
    pimpl(std::allocator_arg_t, 
          const allocator_type& alloc, 
          defer_construct_tag ) noexcept;
    template <typename... Args, typename = typename std::enable_if<std::is_constructible<value_type, Args...>::value>::type>
    explicit pimpl(Args&&... args);
    template <typename... Args, typename = typename std::enable_if<std::is_constructible<value_type, Args...>::value>::type>
    pimpl(std::allocator_arg_t, const allocator_type& alloc, Args&&... args);
    pimpl(const pimpl &other) = delete;
    pimpl(pimpl &&other) noexcept;
    ~pimpl();

    pimpl& operator=(const pimpl& other) = delete;
    pimpl& operator=(pimpl&& other) noexcept;

    reference operator*() noexcept;
    const_reference operator*() const noexcept;
    pointer operator->() noexcept;
    const_pointer operator->() const noexcept;

    operator bool() const noexcept;

    void swap(pimpl& other) noexcept;
    void reset() noexcept;

    template <typename... Args>
    void emplace(Args&&... args);

    pointer get() noexcept;
    const_pointer get() const noexcept;

    allocator_type get_allocator() const noexcept;

private:
    using allocator_traits = std::allocator_traits<allocator_type>;

    XMIPP4_NO_UNIQUE_ADDRESS allocator_type m_allocator;
    pointer m_responsibility;

};

template <typename T, typename Alloc>
void swap(pimpl<T, Alloc>& x, pimpl<T, Alloc>& y) noexcept;

} // namespace memory
} // namespace xmipp4

#include "pimpl.inl"
