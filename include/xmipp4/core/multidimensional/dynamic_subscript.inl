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
 * @file dynamic_subscript.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of dynamic_subscript.hpp
 * @date 2025-02-03
 * 
 */

#include "dynamic_subscript.hpp"

#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

XMIPP4_INLINE_CONSTEXPR
dynamic_subscript::dynamic_subscript(ellipsis_tag) noexcept
    : m_type(subscript_type::ellipsis)
{
}

XMIPP4_INLINE_CONSTEXPR
dynamic_subscript::dynamic_subscript(new_axis_tag) noexcept
    : m_type(subscript_type::new_axis)
{
}

template <typename I, typename>
XMIPP4_INLINE_CONSTEXPR
dynamic_subscript::dynamic_subscript(I index) noexcept
    : m_data{index}
    , m_type(subscript_type::index)
{
}

template <typename Start, typename Stop, typename Step>
XMIPP4_INLINE_CONSTEXPR
dynamic_subscript::dynamic_subscript(const slice<Start, Stop, Step> &slice) noexcept
    : m_data{
        static_cast<std::ptrdiff_t>(slice.get_start()), 
        static_cast<std::ptrdiff_t>(slice.get_count()), 
        static_cast<std::ptrdiff_t>(slice.get_step())
    }
    , m_type(subscript_type::slice)
{
}

XMIPP4_INLINE_CONSTEXPR
dynamic_subscript::subscript_type 
dynamic_subscript::get_subscript_type() const noexcept
{
    return m_type;
}

inline
std::ptrdiff_t dynamic_subscript::get_index() const
{
    if (m_type != subscript_type::index)
    {
        std::ostringstream oss;
        oss << "Cannot call get_index on a " << *this;
        throw std::logic_error(oss.str());
    }

    return m_data[0];
}

inline
dynamic_slice dynamic_subscript::get_slice() const
{
    if (m_type != subscript_type::slice)
    {
        std::ostringstream oss;
        oss << "Cannot call get_slice on a " << *this;
        throw std::logic_error(oss.str());
    }

    return make_slice(m_data[0], m_data[1], m_data[2]);
}



template <typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, 
                                  const dynamic_subscript &subscript )
{
    os << "dynamic_subscript(";
    visit(
        [&os] (auto x) -> void
        {
            os << x;
        },
        subscript
    );
    return os << ")";
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
auto visit(F&& func, const dynamic_subscript &subscript)
{
    switch (subscript.get_subscript_type())
    {
    case dynamic_subscript::subscript_type::ellipsis:
        return std::forward<F>(func)(ellipsis());
    case dynamic_subscript::subscript_type::new_axis:
        return std::forward<F>(func)(new_axis());
    case dynamic_subscript::subscript_type::index:
        return std::forward<F>(func)(subscript.get_index());
    case dynamic_subscript::subscript_type::slice:
        return std::forward<F>(func)(subscript.get_slice());
    }
}

} // namespace multidimensional
} // namespace xmipp4
