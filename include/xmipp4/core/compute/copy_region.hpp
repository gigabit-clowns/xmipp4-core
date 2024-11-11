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
 * @file copy_region.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the compute::copy_region class
 * @date 2024-11-11
 * 
 */

#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class copy_region
{
public:
    XMIPP4_CONSTEXPR copy_region() noexcept;
    XMIPP4_CONSTEXPR
    copy_region(std::size_t source_offset, 
                std::size_t destination_offset,
                std::size_t count ) noexcept;
    copy_region(const copy_region &other) = default;
    copy_region(copy_region &&other) = default;
    ~copy_region() = default;

    copy_region& operator=(const copy_region &other) = default;
    copy_region& operator=(copy_region &&other) = default;

    XMIPP4_CONSTEXPR std::size_t get_source_offset() const noexcept;
    XMIPP4_CONSTEXPR std::size_t get_destination_offset() const noexcept;
    XMIPP4_CONSTEXPR std::size_t get_count() const noexcept;

private:
    std::size_t m_source_offset;
    std::size_t m_destination_offset;
    std::size_t m_count;

};

XMIPP4_CONSTEXPR 
copy_region as_bytes(const copy_region &region,
                     std::size_t element_size ) noexcept;

} // namespace compute
} // namespace xmipp4

#include "copy_region.inl"
