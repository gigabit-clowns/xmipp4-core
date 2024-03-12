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
 * @file pimpl.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of pimpl.hpp
 * @date 2024-03-10
 * 
 */

#include "pimpl.hpp"

#include "propagate_allocator.hpp"

namespace xmipp4 
{
namespace memory
{

template <typename T, typename Alloc>
inline
pimpl<T, Alloc>::pimpl(defer_construct_tag) noexcept
    : pimpl(defer_construct, {})
{
}

template <typename T, typename Alloc>
inline
pimpl<T, Alloc>::pimpl(defer_construct_tag, 
                       const allocator_type& alloc ) noexcept
    : m_allocator(alloc)
    , m_responsibility(nullptr)
{
}

template <typename T, typename Alloc>
template <typename... Args, typename>
inline 
pimpl<T, Alloc>::pimpl(Args&&... args)
    : pimpl(std::allocator_arg, {}, std::forward<Args>(args)...)
{
}

template <typename T, typename Alloc>
template <typename... Args, typename>
inline 
pimpl<T, Alloc>::pimpl(std::allocator_arg_t, 
                       const allocator_type& alloc, 
                       Args&&... args )
    : m_allocator(alloc)
    , m_responsibility(allocator_traits::allocate(m_allocator, 1UL))
{
    allocator_traits::construct(
        m_allocator,
        m_responsibility,
        std::forward<Args>(args)...
    );
}

template <typename T, typename Alloc>
inline pimpl<T, Alloc>::pimpl(const pimpl& other)
    : pimpl(other, other.m_allocator)
{
}

template <typename T, typename Alloc>
inline pimpl<T, Alloc>::pimpl(const pimpl &other, const allocator_type& alloc)
    : pimpl(defer_construct, alloc)
{
    if (other)
    {
        emplace(*other);
    }
}

template <typename T, typename Alloc>
inline pimpl<T, Alloc>::pimpl(pimpl&& other) noexcept
    : m_allocator(std::move(other.m_allocator))
    , m_responsibility(other.m_responsibility)
{
    other.m_responsibility = nullptr;
}

template <typename T, typename Alloc>
inline pimpl<T, Alloc>::~pimpl()
{
    reset();
}

template <typename T, typename Alloc>
inline pimpl<T, Alloc>& pimpl<T, Alloc>::operator=(const pimpl& other)
{
    propagate_allocator_on_copy_assign(m_allocator, other.m_allocator);
    
    if (other)
    {
        emplace(*other);
    }
    else
    {
        reset();
    }
    
    return *this;
}

template <typename T, typename Alloc>
inline pimpl<T, Alloc>& pimpl<T, Alloc>::operator=(pimpl&& other) noexcept
{
    pimpl tmp(defer_construct, m_allocator);
    swap_responsibility(tmp);

    propagate_allocator_on_move_assign(m_allocator, other.m_allocator);
    swap_responsibility(other);

    return *this;
}

template <typename T, typename Alloc>
inline typename pimpl<T, Alloc>::reference 
pimpl<T, Alloc>::operator*() noexcept
{
    return *get();
}

template <typename T, typename Alloc>
inline typename pimpl<T, Alloc>::const_reference 
pimpl<T, Alloc>::operator*() const noexcept
{
    return *get();
}

template <typename T, typename Alloc>
inline typename pimpl<T, Alloc>::pointer 
pimpl<T, Alloc>::operator->() noexcept
{
    return get();
}

template <typename T, typename Alloc>
inline typename pimpl<T, Alloc>::const_pointer 
pimpl<T, Alloc>::operator->() const noexcept
{
    return get();
}

template <typename T, typename Alloc>
inline pimpl<T, Alloc>::operator bool() const noexcept
{
    return m_responsibility != nullptr;
}

template <typename T, typename Alloc>
inline void pimpl<T, Alloc>::swap(pimpl& other) noexcept
{
    propagate_allocator_on_swap(m_allocator, other.m_allocator);
    swap_responsibility(other);
}

template <typename T, typename Alloc>
inline void pimpl<T, Alloc>::reset() noexcept
{
    if (m_responsibility)
    {
        allocator_traits::destroy(
            m_allocator,
            m_responsibility
        );
        allocator_traits::deallocate(
            m_allocator,
            m_responsibility,
            1UL
        );
        m_responsibility = nullptr;
    }
}

template <typename T, typename Alloc>
template <typename... Args>
inline void pimpl<T, Alloc>::emplace(Args&&... args)
{
    if(m_responsibility)
    {
        allocator_traits::destroy(
            m_allocator,
            m_responsibility
        );
    }
    else
    {
        m_responsibility = allocator_traits::allocate(
            m_allocator,
            1UL
        );
    }

    try
    {
        allocator_traits::construct(
            m_allocator,
            m_responsibility,
            std::forward<Args>(args)...
        );
    }
    catch(...)
    {
        allocator_traits::deallocate(
            m_allocator,
            m_responsibility,
            1UL
        );
        m_responsibility = nullptr;
        throw;
    }
}

template <typename T, typename Alloc>
inline typename pimpl<T, Alloc>::pointer 
pimpl<T, Alloc>::get() noexcept
{
    return m_responsibility;
}

template <typename T, typename Alloc>
inline typename pimpl<T, Alloc>::const_pointer 
pimpl<T, Alloc>::get() const noexcept
{
    return m_responsibility;
}

template <typename T, typename Alloc>
inline typename pimpl<T, Alloc>::allocator_type
pimpl<T, Alloc>::get_allocator() const noexcept
{
    return m_allocator;
}



template <typename T, typename Alloc>
inline void pimpl<T, Alloc>::swap_responsibility(pimpl& other) noexcept
{
    using std::swap;
    swap(m_responsibility, other.m_responsibility);
}



template <typename T, typename Alloc>
inline void swap(pimpl<T, Alloc>& x, pimpl<T, Alloc>& y) noexcept
{
    x.swap(y);
}

} // namespace memory
} // namespace xmipp4
