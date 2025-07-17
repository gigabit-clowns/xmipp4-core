// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
