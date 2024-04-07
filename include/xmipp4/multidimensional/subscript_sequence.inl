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
 * @file subscript_sequence.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of subscript_sequence.hpp
 * @date 2024-04-03
 * 
 */

#include "subscript_sequence.hpp"

#include <utility>

namespace xmipp4
{
namespace multidimensional
{

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
