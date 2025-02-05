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
 * @file dynamic_layout.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of dynamic_layout.hpp
 * @date 2025-02-03
 * 
 */

#include "dynamic_layout.hpp"

#include "strided_layout_utils.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
dynamic_layout::dynamic_layout(const std::size_t *extents, 
                               const std::ptrdiff_t *strides, 
                               std::size_t rank,
                               std::ptrdiff_t offset )
    : m_offset(offset)
{
    m_axes.reserve(rank);
    for (std::size_t i = 0; i < rank; ++i)
    {
        m_axes.emplace_back(extents[i], strides[i]);
    }
}

inline
dynamic_layout::dynamic_layout(const axis_descriptor *axes, 
                               std::size_t rank,
                               std::ptrdiff_t offset )
    : m_axes(axes, axes + rank)
    , m_offset(offset)
{
}

inline
std::size_t dynamic_layout::get_rank() const noexcept
{
    return m_axes.size();
}

inline
bool dynamic_layout::get_axis(std::size_t index, 
                              axis_descriptor &out ) const noexcept
{
    const bool result = (index < m_axes.size());
    if (result)
    {
        out = m_axes[index];
    }
    return result;
}

inline
std::ptrdiff_t dynamic_layout::get_offset() const noexcept
{
    return m_offset;
}



inline
dynamic_layout dynamic_layout::slice(span<const dynamic_slice> slices)
{
    dynamic_layout result = *this;
    result.slice_inplace(slices);
    return result;
}

inline
dynamic_layout& dynamic_layout::slice_inplace(span<const dynamic_slice> slices)
{
    if (slices.size() >= get_rank())
    {
        throw std::invalid_argument(
            "The number of slices exceeds the number of dimensions"
        );
    }

    for (std::size_t i = 0; i < slices.size(); ++i)
    {
        apply_slice(m_axes[i], slices[i], m_offset);
    }

    return *this;
}

inline
dynamic_layout dynamic_layout::transpose() const
{
    dynamic_layout result = *this;
    result.transpose_inplace();
    return result;
}

inline
dynamic_layout& dynamic_layout::transpose_inplace() noexcept
{
    transpose_layout_inplace(m_axes.begin(), m_axes.end());
    return *this;
}

inline
dynamic_layout dynamic_layout::matrix_transpose() const
{
    dynamic_layout result = *this;
    result.matrix_transpose_inplace();
    return result;
}

inline
dynamic_layout& dynamic_layout::matrix_transpose_inplace() noexcept
{
    const auto index0 = m_axes.size() - 1;
    const auto index1 = index0 - 1;
    std::swap(m_axes[index0], m_axes[index1]);
    return *this;
}

inline
dynamic_layout dynamic_layout::squeeze() const
{
    dynamic_layout result = *this;
    result.squeeze_inplace();
    return result;
}

inline
dynamic_layout& dynamic_layout::squeeze_inplace() noexcept
{
    m_axes.erase(
        squeeze_layout_inplace(m_axes.begin(), m_axes.end()), 
        m_axes.end()
    );

    return *this;
}

} // namespace multidimensional
} // namespace xmipp4

