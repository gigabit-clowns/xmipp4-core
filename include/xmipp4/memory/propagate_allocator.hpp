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
 * @file propagate_allocator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides functions that conditionally propagate allocator assignments
 * based on std::allocator_traits
 * @date 2024-03-12
 * 
 */

namespace xmipp4 
{
namespace memory
{

/**
 * @brief Swap allocators when required.
 * 
 * Swap allocators only if std::allocator_traits::propagate_container_swap
 * evaluates to std::true_type
 * 
 * @tparam Alloc Allocator type
 * @param x One of the allocators
 * @param y The other allocator
 */
template <typename Alloc>
void propagate_allocator_on_swap(Alloc& x, Alloc& y);

/**
 * @brief Move allocator when required.
 * 
 * Move allocator only if std::allocator_traits::propagate_container_move_assign
 * evaluates to std::true_type
 * 
 * @tparam Alloc Allocator type
 * @param to The allocator to be assigned to
 * @param from The allocator to be assigned from
 */
template <typename Alloc>
void propagate_allocator_on_move_assign(Alloc& to, Alloc& from);

/**
 * @brief Copy allocator when required.
 * 
 * Copy allocator only if std::allocator_traits::propagate_container_copy_assign
 * evaluates to std::true_type
 * 
 * @tparam Alloc Allocator type
 * @param to The allocator to be assigned to
 * @param from The allocator to be assigned from
 */
template <typename Alloc>
void propagate_allocator_on_copy_assign(Alloc& to, const Alloc& from);

} // namespace memory
} // namespace xmipp4

#include "propagate_allocator.inl"
