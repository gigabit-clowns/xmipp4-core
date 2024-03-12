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

namespace xmipp4 
{
namespace memory
{

template <typename Alloc>
inline
typename std::enable_if<std::allocator_traits<Alloc>::propagate_on_container_swap::value>::type
propagate_allocator_on_swap(Alloc& x, Alloc& y)
{
    using std::swap;
    swap(x, y);
}

template <typename Alloc>
inline
typename std::enable_if<!std::allocator_traits<Alloc>::propagate_on_container_swap::value>::type
propagate_allocator_on_swap(Alloc&, Alloc&) noexcept
{
}

template <typename Alloc>
inline
typename std::enable_if<std::allocator_traits<Alloc>::propagate_on_container_move_assignment::value>::type
propagate_allocator_on_move_assign(Alloc& to, Alloc& from)
{
    to = std::move(from);
}

template <typename Alloc>
inline
typename std::enable_if<!std::allocator_traits<Alloc>::propagate_on_container_move_assignment::value>::type
propagate_allocator_on_move_assign(Alloc&, Alloc&) noexcept
{
}

template <typename Alloc>
inline
typename std::enable_if<std::allocator_traits<Alloc>::propagate_on_container_copy_assignment::value>::type
propagate_allocator_on_copy_assign(Alloc& to, const Alloc& from)
{
    to = from;
}

template <typename Alloc>
inline
typename std::enable_if<!std::allocator_traits<Alloc>::propagate_on_container_copy_assignment::value>::type
propagate_allocator_on_copy_assign(Alloc&, const Alloc&) noexcept
{
}

} // namespace memory
} // namespace xmipp4
