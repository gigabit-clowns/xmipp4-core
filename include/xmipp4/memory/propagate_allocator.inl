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
 * @file propagate_allocator.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of propagate_allocator.hpp
 * @date 2024-03-12
 * 
 */

#include "pimpl.hpp"

#include <memory>

namespace xmipp4 
{
namespace memory
{
namespace detail
{

template <typename Alloc>
inline 
void propagate_allocator_on_swap(Alloc&, Alloc&, std::false_type)
{
    return; //No-op
}

template <typename Alloc>
inline 
void propagate_allocator_on_swap(Alloc& x, Alloc& y, std::true_type)
{
    using std::swap;
    swap(x, y);
}



template <typename Alloc>
inline 
void propagate_allocator_on_move_assign(Alloc&, Alloc&, std::false_type)
{
    return; //No-op
}

template <typename Alloc>
inline 
void propagate_allocator_on_move_assign(Alloc& to, Alloc& from, std::true_type)
{
    to = std::move(from);
}



template <typename Alloc>
inline 
void propagate_allocator_on_copy_assign(Alloc&, const Alloc&, std::false_type)
{
    return; //No-op
}

template <typename Alloc>
inline 
void propagate_allocator_on_copy_assign(Alloc& to, const Alloc& from, std::true_type)
{
    to = from;
}

} // namespace detail


template <typename Alloc>
inline void propagate_allocator_on_swap(Alloc& x, Alloc& y)
{
    detail::propagate_allocator_on_swap(
        x, y,
        typename std::allocator_traits<Alloc>::propagate_on_container_swap()
    );
}

template <typename Alloc>
inline void propagate_allocator_on_move_assign(Alloc& to, Alloc& from)
{
    detail::propagate_allocator_on_move_assign(
        to, from,
        typename std::allocator_traits<Alloc>::propagate_on_container_move_assignment()
    );
}

template <typename Alloc>
inline void propagate_allocator_on_copy_assign(Alloc& to, const Alloc& from)
{
    detail::propagate_allocator_on_copy_assign(
        to, from,
        typename std::allocator_traits<Alloc>::propagate_on_container_copy_assignment()
    );
}

} // namespace memory
} // namespace xmipp4
