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
 * @file object_reference.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides object_reference class
 * @date 2023-09-28
 * 
 */

#include "../platform/constexpr.hpp"

#include <typeindex>
#include <type_traits>

namespace xmipp4 
{

template <bool IsConst>
class object_reference
{
public:
    using is_const = std::integral_constant<bool, IsConst>;
    
    template <typename T>
    using reference = typename std::conditional<is_const::value, const T&, T&>::type;

    XMIPP4_CONSTEXPR object_reference() noexcept;
    template <typename T>
    XMIPP4_CONSTEXPR object_reference(reference<T> obj) noexcept;
    object_reference(const object_reference& other) = default;
    object_reference(object_reference&& other) = default;
    ~object_reference() = default;

    object_reference& operator=(const object_reference& other) = default;
    object_reference& operator=(object_reference&& other) = default;

    XMIPP4_CONSTEXPR operator bool() const noexcept;
    XMIPP4_CONSTEXPR bool has_value() const noexcept;

    template <typename T>
    reference<T> get() const;

private:
    using void_pointer = typename std::conditional<is_const::value, const void*, void*>::type;

    void_pointer m_reference;
    std::type_index m_type;

};

template <typename T, bool IsConst> 
typename object_reference<IsConst>::template reference<T> 
object_cast(const object_reference<IsConst>& reference);

} // namespace xmipp4

#include "object_reference.inl"
