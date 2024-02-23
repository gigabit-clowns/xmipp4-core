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
 * @file static_extent.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines static_extent class
 * @date 2024-02-22
 * 
 */

#include "../platform/constexpr.hpp"

#include <utility>
#include <cstddef>

namespace xmipp4 
{

namespace detail
{

template<std::size_t i, typename S>
class static_extent_impl;

}

template<std::size_t... Sizes>
class static_extent 
{
public:
    template <typename... Args>
    XMIPP4_CONSTEXPR static_extent(Args&&... args) noexcept;
    static_extent(const static_extent& other) = default;
    static_extent(static_extent&& other) = default;
    ~static_extent() = default;

    static_extent& operator=(const static_extent& other) = default;
    static_extent& operator=(static_extent&& other) = default;

    std::size_t get_size(std::size_t index) const;
    template<std::size_t i>
    XMIPP4_CONSTEXPR std::size_t get_size() const noexcept;

    static XMIPP4_CONSTEXPR std::size_t get_rank() noexcept;

private:
    using size_sequence = std::integer_sequence<std::size_t, Sizes...>;

    detail::static_extent_impl<0UL, size_sequence> m_impl;

};

} //namespace xmipp4

#include "static_extent.inl"