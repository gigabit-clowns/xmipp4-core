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
 * @file axis_descriptor.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of axis_descriptor
 * @date 2023-08-13
 * 
 */

#include "axis_descriptor.hpp"

namespace xmipp4
{
namespace multidimensional
{

XMIPP4_INLINE_CONSTEXPR axis_descriptor::axis_descriptor(std::size_t count, 
                                                         std::ptrdiff_t step ) noexcept
    : m_count(count)
    , m_step(step)
{   
}
    
XMIPP4_INLINE_CONSTEXPR 
bool axis_descriptor::operator==(const axis_descriptor& other) const noexcept
{
    return m_count == other.m_count && 
           m_step == other.m_step;
}

XMIPP4_INLINE_CONSTEXPR 
bool axis_descriptor::operator!=(const axis_descriptor& other) const noexcept
{
    return !(*this == other);
}

XMIPP4_INLINE_CONSTEXPR_CPP20
void axis_descriptor::swap(axis_descriptor &other) noexcept
{
    std::swap(m_count, other.m_count);
    std::swap(m_step, other.m_step);
}

XMIPP4_INLINE_CONSTEXPR 
void axis_descriptor::set_count(std::size_t count) noexcept
{
    m_count = count;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t axis_descriptor::get_count() const noexcept
{
    return m_count;
}

XMIPP4_INLINE_CONSTEXPR 
void axis_descriptor::set_step(std::ptrdiff_t step) noexcept
{
    m_step = step;
}

XMIPP4_INLINE_CONSTEXPR 
std::ptrdiff_t axis_descriptor::get_step() const noexcept
{
    return m_step;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t axis_descriptor::get_unsigned_step() const noexcept
{
    //FIXME Using a manual abs because std::abs is not constexpr. Use a proper abs function
    const auto step = get_step();
    return static_cast<std::size_t>(step < 0 ? -step : step);
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t axis_descriptor::get_width() const noexcept
{
    return get_unsigned_step()*get_count();
}

XMIPP4_INLINE_CONSTEXPR_CPP20
void swap(axis_descriptor &x, axis_descriptor &y) noexcept
{
    x.swap(y);
}

} // namespace multidimensional
} // namespace xmipp4
