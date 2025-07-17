// SPDX-License-Identifier: GPL-3.0-only

#include "subscript_sequence.hpp"

#include <utility>

namespace xmipp4
{
namespace multidimensional
{

XMIPP4_INLINE_CONSTEXPR
std::size_t subscript_sequence<>::size() noexcept
{
    return 0UL;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t subscript_sequence<>::consumption() noexcept
{
    return 0UL;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t subscript_sequence<>::production() noexcept
{
    return 0UL;
}



template <typename Head, typename... Tail>
template<typename First, typename... Rest>
XMIPP4_INLINE_CONSTEXPR
subscript_sequence<Head, Tail...>::subscript_sequence(First&& first, Rest&&... rest)
    : m_head(std::forward<First>(first))
    , m_tail(std::forward<Rest>(rest)...)
{
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
typename subscript_sequence<Head, Tail...>::head_type& 
subscript_sequence<Head, Tail...>::head() noexcept
{
    return m_head;
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
const typename subscript_sequence<Head, Tail...>::head_type& 
subscript_sequence<Head, Tail...>::head() const noexcept
{
    return m_head;
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
typename subscript_sequence<Head, Tail...>::tail_type& 
subscript_sequence<Head, Tail...>::tail() noexcept
{
    return m_tail;
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
const typename subscript_sequence<Head, Tail...>::tail_type& 
subscript_sequence<Head, Tail...>::tail() const noexcept
{
    return m_tail;
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
std::size_t subscript_sequence<Head, Tail...>::size() noexcept
{
    return sizeof...(Tail) + 1;
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
std::size_t subscript_sequence<Head, Tail...>::consumption() noexcept
{
    return tail_type::consumption() + axis_consumption<head_type>::value;
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
std::size_t subscript_sequence<Head, Tail...>::production() noexcept
{
    return tail_type::production() + axis_production<head_type>::value;
}





XMIPP4_INLINE_CONSTEXPR
new_axis_tag new_axis() noexcept
{
    return new_axis_tag();
}

XMIPP4_INLINE_CONSTEXPR
ellipsis_tag ellipsis() noexcept
{
    return ellipsis_tag();
}





template <typename... Subscripts>
XMIPP4_INLINE_CONSTEXPR
subscript_sequence<typename std::decay<Subscripts>::type...> 
make_subscript_sequence(Subscripts&&... subscripts)
{
    return subscript_sequence<typename std::decay<Subscripts>::type...>(
        std::forward<Subscripts>(subscripts)...
    );
}

} // namespace multidimensional
} // namespace xmipp4
