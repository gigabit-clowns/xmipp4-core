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

#include "slice_sequence.hpp"

#include <utility>

namespace xmipp4
{
namespace multidimensional
{

template <typename Head, typename... Tail>
template<typename First, typename... Rest>
XMIPP4_INLINE_CONSTEXPR
slice_sequence<Head, Tail...>::slice_sequence(First&& first, Rest&&... rest)
    : m_head(std::forward<First>(first))
    , m_tail(std::forward<Rest>(rest)...)
{
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
typename slice_sequence<Head, Tail...>::head_type& 
slice_sequence<Head, Tail...>::head() noexcept
{
    return m_head;
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
const typename slice_sequence<Head, Tail...>::head_type& 
slice_sequence<Head, Tail...>::head() const noexcept
{
    return m_head;
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
typename slice_sequence<Head, Tail...>::tail_type& 
slice_sequence<Head, Tail...>::tail() noexcept
{
    return m_tail;
}

template <typename Head, typename... Tail>
XMIPP4_INLINE_CONSTEXPR
const typename slice_sequence<Head, Tail...>::tail_type& 
slice_sequence<Head, Tail...>::tail() const noexcept
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





template <typename... Slices>
XMIPP4_INLINE_CONSTEXPR
slice_sequence<Slices...> make_slice_sequence(const Slices&... slices)
{
    return slice_sequence<Slices...>(slices...);
}

} // namespace multidimensional
} // namespace xmipp4
