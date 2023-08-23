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

#include "allocation_info.hpp"

namespace xmipp4
{
namespace metadata
{

XMIPP4_INLINE_CONSTEXPR 
allocation_info::allocation_info() noexcept
    : m_size(0)
    , m_capacity(0)
{
}

XMIPP4_INLINE_CONSTEXPR 
allocation_info::allocation_info(std::size_t size) noexcept
    : m_size(size)
    , m_capacity(size)
{
}

XMIPP4_INLINE_CONSTEXPR 
allocation_info::allocation_info(std::size_t size, std::size_t capacity) noexcept
    : m_size(size)
    , m_capacity(capacity)
{
}

XMIPP4_INLINE_CONSTEXPR 
void allocation_info::set_size(std::size_t size) noexcept
{
    m_size = size;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t allocation_info::get_size() const noexcept
{
    return m_size;
}

XMIPP4_INLINE_CONSTEXPR 
void allocation_info::set_capacity(std::size_t capacity) noexcept
{
    m_capacity = capacity;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t allocation_info::get_capacity() const noexcept
{
    return m_capacity;
}

} // namespace metadata
} // namespace xmipp4
