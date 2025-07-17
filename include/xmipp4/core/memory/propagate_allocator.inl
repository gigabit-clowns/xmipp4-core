// SPDX-License-Identifier: GPL-3.0-only

#include "propagate_allocator.hpp"

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
