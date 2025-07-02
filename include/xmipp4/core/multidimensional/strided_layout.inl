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
 * @file strided_layout.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of strided_layout.hpp
 * @date 2025-02-03
 * 
 */

#include "strided_layout.hpp"

#include "checks.hpp"

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout::apply_subscripts_helper
{
public:
    strided_layout operator()(const strided_layout &source, 
                              span<const dynamic_subscript> subscripts )
    {
        initialize(source);

        process_subscripts(
            m_axes.begin(), m_axes.end(), 
            subscripts.begin(), subscripts.end()
        );

        return strided_layout(std::move(m_axes), m_offset);
    }

private:
    std::vector<strided_axis> m_axes;
    std::ptrdiff_t m_offset;

    void initialize(const strided_layout &source)
    {
        m_axes = source.m_axes;
        m_offset = source.m_offset;
    }

    template <typename Ite1, typename Ite2>
    void process_subscripts(Ite1 first_axis, Ite1 last_axis, 
                            Ite2 first_subscript, Ite2 last_subscript )
    {
        while (first_axis != last_axis && first_subscript != last_subscript)
        {
            visit(
                [this, &first_axis, &first_subscript, last_subscript] 
                (auto subscript) -> void
                {
                    apply_subscript(
                        first_axis, 
                        first_subscript, 
                        last_subscript, 
                        subscript
                    );
                },
                *first_subscript
            );
        }
        
        if (first_subscript != last_subscript)
        {
            throw std::invalid_argument("Not all subscripts were processed");
        }
    }

    void apply_subscript(std::vector<strided_axis>::iterator &axis_ite, 
                         span<const dynamic_subscript>::iterator &subscript_ite, 
                         span<const dynamic_subscript>::iterator last_subscript, 
                         ellipsis_tag )
    {
        process_subscripts(
            std::make_reverse_iterator(axis_ite), 
            m_axes.rend(),
            std::make_reverse_iterator(std::next(subscript_ite)), // Leave this out.
            std::make_reverse_iterator(last_subscript)
        );
        subscript_ite = last_subscript; // Finishes the calling loop.
    }

    void apply_subscript(std::vector<strided_axis>::iterator &axis_ite, 
                         span<const dynamic_subscript>::iterator &subscript_ite, 
                         span<const dynamic_subscript>::iterator, 
                         new_axis_tag )
    {
        axis_ite = m_axes.insert(axis_ite, make_phantom_axis());
        ++axis_ite;
        ++subscript_ite;
    }

    void apply_subscript(std::vector<strided_axis>::iterator &axis_ite,
                         span<const dynamic_subscript>::iterator &subscript_ite, 
                         span<const dynamic_subscript>::iterator, 
                         std::ptrdiff_t index)
    {

        const auto sanitized = sanitize_index(index, axis_ite->get_extent());
        apply_index(*axis_ite, m_offset, sanitized);
        axis_ite = m_axes.erase(axis_ite);
        ++subscript_ite;
    }

    void apply_subscript(std::vector<strided_axis>::iterator &axis_ite,
                         span<const dynamic_subscript>::iterator &subscript_ite, 
                         span<const dynamic_subscript>::iterator, 
                         const dynamic_slice &slice)
    {
        const auto sanitized_slice = sanitize_slice(slice, axis_ite->get_extent());
        apply_slice(*axis_ite, m_offset, sanitized_slice);
        ++axis_ite;
        ++subscript_ite;
    }

    void apply_subscript(std::vector<strided_axis>::reverse_iterator&, 
                         span<const dynamic_subscript>::reverse_iterator&, 
                         span<const dynamic_subscript>::reverse_iterator, 
                         ellipsis_tag )
    {
        throw std::invalid_argument("Two ellipsis tags were encountered");
    }

    void apply_subscript(std::vector<strided_axis>::reverse_iterator &axis_ite, 
                         span<const dynamic_subscript>::reverse_iterator &subscript_ite, 
                         span<const dynamic_subscript>::reverse_iterator, 
                         new_axis_tag )
    {
        auto base = axis_ite.base();
        base = m_axes.insert(base, make_phantom_axis());
        axis_ite = std::make_reverse_iterator(base);
        ++subscript_ite;
    }

    void apply_subscript(std::vector<strided_axis>::reverse_iterator &axis_ite,
                         span<const dynamic_subscript>::reverse_iterator &subscript_ite, 
                         span<const dynamic_subscript>::reverse_iterator, 
                         std::ptrdiff_t index)
    {
        const auto sanitized = sanitize_index(index, axis_ite->get_extent());
        apply_index(*axis_ite, m_offset, sanitized);
        
        auto base = std::prev(axis_ite.base());
        base = m_axes.erase(base);
        axis_ite = std::make_reverse_iterator(base);
        ++subscript_ite;
    }

    void apply_subscript(std::vector<strided_axis>::reverse_iterator &axis_ite,
                         span<const dynamic_subscript>::reverse_iterator &subscript_ite, 
                         span<const dynamic_subscript>::reverse_iterator, 
                         const dynamic_slice &slice)
    {
        const auto sanitized_slice = sanitize_slice(slice, axis_ite->get_extent());
        apply_slice(*axis_ite, m_offset, sanitized_slice);
        ++axis_ite;
        ++subscript_ite;
    }

};





inline
strided_layout::strided_layout(std::vector<strided_axis> &&axes,
                               std::ptrdiff_t offset ) noexcept
    : m_axes(std::move(axes))
    , m_offset(offset)
{
}

inline
strided_layout::strided_layout(const std::size_t *extents, 
                               std::size_t rank,
                               std::ptrdiff_t offset )
    : m_offset(offset)
{
    m_axes.reserve(rank);
    for (std::size_t i = 0; i < rank; ++i)
    {
        m_axes.emplace_back(extents[i], 0);
    }

    std::size_t stride = 1;
    for(auto ite = m_axes.rbegin(); ite != m_axes.rend(); ++ite)
    {
        ite->set_stride(stride);
        stride *= ite->get_extent();
    }
}

inline
strided_layout::strided_layout(const std::size_t *extents, 
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
strided_layout::strided_layout(const strided_axis *axes, 
                               std::size_t rank,
                               std::ptrdiff_t offset )
    : m_axes(axes, axes + rank)
    , m_offset(offset)
{
}

XMIPP4_NODISCARD inline
std::size_t strided_layout::get_rank() const noexcept
{
    return m_axes.size();
}

inline
std::size_t strided_layout::get_axes(std::size_t *extents, 
                                     std::ptrdiff_t *strides,
                                     std::size_t count ) const noexcept
{
    if (count > m_axes.size())
    {
        count = m_axes.size();
    }

    for (std::size_t i = 0; i < count; ++i)
    {
        const auto &axis = m_axes[i];
        if(extents)
        {
            extents[i] = axis.get_extent();
        }
        if(strides)
        {
            strides[i] = axis.get_stride();
        }
    }

    return count;
}

inline
bool strided_layout::get_axis(std::size_t index, 
                              strided_axis &out ) const noexcept
{
    const bool result = (index < m_axes.size());
    if (result)
    {
        out = m_axes[index];
    }
    return result;
}

XMIPP4_NODISCARD inline
std::ptrdiff_t strided_layout::get_offset() const noexcept
{
    return m_offset;
}



XMIPP4_NODISCARD inline
strided_layout 
strided_layout::apply_subscripts(span<const dynamic_subscript> subscripts) const
{
    apply_subscripts_helper helper;
    return helper(*this, subscripts);
}

XMIPP4_NODISCARD inline
strided_layout strided_layout::transpose() const
{
    std::vector<strided_axis> axes;
    axes.reserve(m_axes.size());

    std::reverse_copy(
        m_axes.cbegin(), m_axes.cend(),
        std::back_inserter(axes)
    );

    return strided_layout(std::move(axes), m_offset);
}

XMIPP4_NODISCARD inline
strided_layout strided_layout::permute(span<const std::size_t> order) const
{
    const auto count = m_axes.size();
    check_axis_permutation(order.begin(), order.end(), count);

    std::vector<strided_axis> axes;
    axes.reserve(count);

    for(std::size_t i = 0; i < count; ++i)
    {
        axes.push_back(m_axes[order[i]]);
    }

    return strided_layout(std::move(axes), m_offset);
}

XMIPP4_NODISCARD inline
strided_layout 
strided_layout::swap_axes(std::size_t axis1, std::size_t axis2) const
{
    if (axis1 >= m_axes.size())
    {
        throw std::invalid_argument("axis1 exceeds bounds");
    }
    if (axis2 >= m_axes.size())
    {
        throw std::invalid_argument("axis2 exceeds bounds");
    }

    auto axes = m_axes;
    std::swap(axes[axis1], axes[axis2]);

    return strided_layout(std::move(axes), m_offset);
}

XMIPP4_NODISCARD inline
strided_layout strided_layout::squeeze() const
{
    std::vector<strided_axis> axes;

    std::copy_if(
        m_axes.cbegin(), m_axes.cend(),
        std::back_inserter(axes),
        is_significant
    );

    return strided_layout(std::move(axes), m_offset);
}

inline
void strided_layout::broadcast_dry(std::vector<std::size_t> &extents) const
{
    if (m_axes.size() < extents.size())
    {
        std::ostringstream oss;
        oss << "Can not broadcast shape with " << extents.size()
            << " axes into a layout of " << m_axes.size() << " axes.";
        throw std::invalid_argument(oss.str());
    }
    
    if (m_axes.size() > extents.size())
    {
        const std::size_t old_size = extents.size();
        const std::size_t new_size = m_axes.size();
        const std::size_t padding = new_size - old_size;
        extents.insert(
            extents.cbegin(),
            padding,
            1UL
        );
    }

    const std::size_t count = extents.size(); 
    for(std::size_t i = 0; i < count; ++i)
    {
        if (!multidimensional::broadcast_dry(m_axes[i], extents[i]))
        {
            std::ostringstream oss;
            oss << "Can not broadcast extent " << extents[i]
                << "into an axis of extent " << m_axes[i].get_extent();
            throw std::invalid_argument(oss.str());
        }
    }
}

XMIPP4_NODISCARD inline
strided_layout 
strided_layout::broadcast_to(span<const std::size_t> extents) const
{

    if (m_axes.size() > extents.size())
    {
        std::ostringstream oss;
        oss << "Can not broadcast layout with " << m_axes.size()
            << " axes into a shape of " << extents.size() << " axes.";
        throw std::invalid_argument(oss.str());
    }

    std::vector<strided_axis> axes;
    axes.reserve(extents.size());
    
    const auto padding = extents.size() - m_axes.size();
    std::fill_n(
        std::back_inserter(axes), padding,
        make_phantom_axis()
    );
    std::copy(
        m_axes.cbegin(), m_axes.cend(),
        std::back_inserter(axes)
    );

    const std::size_t count = extents.size(); 
    for(std::size_t i = 0; i < count; ++i)
    {
        if (!multidimensional::broadcast_to(axes[i], extents[i]))
        {
            std::ostringstream oss;
            oss << "Can not broadcast axis of extent " << axes[i].get_extent()
                << "into an extent of " << extents[i] << ".";
            throw std::invalid_argument(oss.str());
        }
    }

    return strided_layout(std::move(axes), m_offset);
}

} // namespace multidimensional
} // namespace xmipp4
