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

#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4
{
namespace metadata
{

class allocation_context
{
public:
    XMIPP4_CONSTEXPR allocation_context() noexcept;
    explicit XMIPP4_CONSTEXPR allocation_context(std::size_t size) noexcept;
    XMIPP4_CONSTEXPR allocation_context(std::size_t size, std::size_t capacity) noexcept;
    allocation_context(const allocation_context& other) = default;
    allocation_context(allocation_context&& other) = default;
    ~allocation_context() = default;

    XMIPP4_CONSTEXPR void swap(allocation_context& other) noexcept;

    allocation_context& operator=(const allocation_context& other) = default;
    allocation_context& operator=(allocation_context&& other) = default;

    XMIPP4_CONSTEXPR void set_size(std::size_t size) noexcept;
    XMIPP4_CONSTEXPR std::size_t get_size() const noexcept;

    XMIPP4_CONSTEXPR void set_capacity(std::size_t capacity) noexcept;
    XMIPP4_CONSTEXPR std::size_t get_capacity() const noexcept;

private:
    std::size_t m_size;
    std::size_t m_capacity;

};

XMIPP4_CONSTEXPR bool operator==(const allocation_context& x, const allocation_context& y) noexcept;
XMIPP4_CONSTEXPR bool operator!=(const allocation_context& x, const allocation_context& y) noexcept;

XMIPP4_CONSTEXPR void swap(allocation_context& x, allocation_context& y) noexcept;


} // namespace metadata
} // namespace xmipp4

#include "allocation_context.inl"
