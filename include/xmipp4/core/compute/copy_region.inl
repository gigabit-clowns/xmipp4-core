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
 * @file copy_region.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of copy_region.hpp
 * @date 2024-11-11
 * 
 */

#include "copy_region.hpp"

namespace xmipp4 
{
namespace compute
{

XMIPP4_INLINE_CONSTEXPR 
copy_region::copy_region() noexcept
    : m_source_offset(0)
    , m_destination_offset(0)
    , m_count(0)
{
}

XMIPP4_INLINE_CONSTEXPR 
copy_region::copy_region(std::size_t source_offset, 
                         std::size_t destination_offset,
                         std::size_t count ) noexcept
    : m_source_offset(source_offset)
    , m_destination_offset(destination_offset)
    , m_count(count)
{
}


XMIPP4_INLINE_CONSTEXPR
std::size_t copy_region::get_source_offset() const noexcept
{
    return m_source_offset;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t copy_region::get_destination_offset() const noexcept
{
    return m_destination_offset;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t copy_region::get_count() const noexcept
{
    return m_count;
}



XMIPP4_INLINE_CONSTEXPR
copy_region as_bytes(const copy_region &region,
                     std::size_t element_size ) noexcept
{
    return copy_region(
        region.get_source_offset()*element_size,
        region.get_destination_offset()*element_size,
        region.get_count()*element_size
    );
}

} // namespace compute
} // namespace xmipp4
