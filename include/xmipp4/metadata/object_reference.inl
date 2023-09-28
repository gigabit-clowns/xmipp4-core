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
 * @file object_reference.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of object_reference.hpp
 * @date 2023-09-28
 * 
 */

#include "object_reference.hpp"

#include <stdexcept>

namespace xmipp4 
{

template <bool IsConst>
XMIPP4_INLINE_CONSTEXPR 
object_reference<IsConst>::object_reference() noexcept
    : m_reference(nullptr)
    , m_type(typeid(void))
{
}

template <bool IsConst>
template <typename T>
XMIPP4_INLINE_CONSTEXPR 
object_reference<IsConst>::object_reference(reference<T> obj) noexcept
    : m_reference(&obj)
    , m_type(typeid(T))
{
}

template <bool IsConst>
XMIPP4_INLINE_CONSTEXPR 
object_reference<IsConst>::operator bool() const noexcept
{
    return has_value();
}

template <bool IsConst>
XMIPP4_INLINE_CONSTEXPR 
bool object_reference<IsConst>::has_value() const noexcept
{
    return m_reference;
}

template <bool IsConst>
template <typename T>
inline
typename object_reference<IsConst>::template reference<T>
object_reference<IsConst>::get() const
{
    if (m_type != typeid(T))
        throw std::bad_cast();

    return *static_cast<T*>(m_reference);
}

template <typename T, bool IsConst> 
inline
typename object_reference<IsConst>::template reference<T> 
object_cast(const object_reference<IsConst>& reference)
{
    return reference.get<T>();
}

} // namespace xmipp4
