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

#include "dynamic_strided_layout.hpp"

#include "strided_layout_utils.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename It>
inline
dynamic_strided_layout::dynamic_strided_layout(It first, It last)
    : m_axes(first, last)
{
    compute_contiguous_axis_strides(m_axes.begin(), m_axes.end(), column_major());
}

inline
dynamic_strided_layout::dynamic_strided_layout(std::initializer_list<std::size_t> shape)
    : dynamic_strided_layout(shape.begin(), shape.end())
{
}

inline
dynamic_strided_layout::dynamic_strided_layout(const shape_type &shape)
    : dynamic_strided_layout(shape.begin(), shape.end())
{
}

inline
std::size_t dynamic_strided_layout::get_rank() const noexcept
{
    return m_axes.size();
}

inline
const axis_descriptor& dynamic_strided_layout::get_axis(std::size_t i) const
{
    return m_axes.at(i);
}

inline
std::size_t dynamic_strided_layout::get_extent(std::size_t i) const
{
    return get_axis(i).get_extent();
}

inline
std::ptrdiff_t dynamic_strided_layout::get_stride(std::size_t i) const
{
    return get_axis(i).get_stride();
}

inline
std::size_t dynamic_strided_layout::get_required_storage_size() const noexcept
{
    if (m_axes.empty()) return 0UL;
    const auto minor_axis = find_max_stride(m_axes.cbegin(), m_axes.cend());
    return minor_axis->get_width();
}

} // namespace multidimensional
} // namespace xmipp4

