// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include <xmipp4/core/multidimensional/checks.hpp>

#include "strided_axis.hpp"

#include <algorithm>
#include <vector>
#include <numeric>

namespace xmipp4 
{
namespace multidimensional
{

class apply_subscripts_helper
{
public:
    template <typename BidirIt1, typename BidirIt2>
    static
    std::vector<strided_axis> 
    process(BidirIt1 first_subscript, BidirIt1 last_subscript,
            BidirIt2 first_axis, BidirIt2 last_axis,
            std::ptrdiff_t offset )
    {
        std::vector<strided_axis> axes; 

        process_forwards(
            first_subscript, last_subscript,
            first_axis, last_axis,
            offset,
            axes, axes.begin()
        );

        return axes;
    }

private:
    template <typename BidirIt1, typename BidirIt2>
    static
    void process_forwards(BidirIt1 first_subscript, BidirIt1 last_subscript,
                          BidirIt2 first_axis, BidirIt2 last_axis, 
                          std::ptrdiff_t &offset,
                          std::vector<strided_axis> &axes,
                          std::vector<strided_axis>::iterator head_ite )
    {
        while (first_axis != last_axis && first_subscript != last_subscript)
        {
            const auto &subscript = *first_subscript;
            const auto &axis = *first_axis;

            switch (subscript.get_subscript_type())
            {
            case dynamic_subscript::subscript_type::ellipsis:
                process_backwards(
                    std::next(first_subscript), last_subscript,
                    first_axis, last_axis,
                    offset,
                    axes,
                    head_ite
                );
                first_subscript = last_subscript; // Finishes processing
                first_axis = last_axis;
                break;

            case dynamic_subscript::subscript_type::new_axis:
                head_ite = axes.insert(head_ite, make_phantom_axis());
                ++first_subscript;
                ++head_ite;
                break;

            case dynamic_subscript::subscript_type::index:
                apply_index(axis, offset, subscript.get_index());
                ++first_subscript;
                ++first_axis;
                break;
            
            case dynamic_subscript::subscript_type::slice:
                head_ite = axes.insert(head_ite, axis);
                apply_slice(*head_ite, offset, subscript.get_slice());
                ++first_subscript;
                ++first_axis;
                ++head_ite;
                break;

            default:
                throw std::logic_error("Unknown subscript type encountered");
                break;
            }
        }
        
        if (first_subscript != last_subscript)
        {
            throw std::invalid_argument("Not all subscripts were processed");
        }

        // Copy reminder
        if (first_axis != last_axis)
        {
            axes.insert(head_ite, first_axis, last_axis);
        }
    }

    template <typename BidirIt1, typename BidirIt2>
    static
    void process_backwards(BidirIt1 first_subscript, BidirIt1 last_subscript,
                           BidirIt2 first_axis, BidirIt2 last_axis, 
                           std::ptrdiff_t &offset,
                           std::vector<strided_axis> &axes,
                           std::vector<strided_axis>::iterator head_ite )
    {
        while (first_axis != last_axis && first_subscript != last_subscript)
        {
            const auto &subscript = *std::prev(last_subscript);
            const auto &axis = *std::prev(last_axis);

            switch (subscript.get_subscript_type())
            {
            case dynamic_subscript::subscript_type::ellipsis:
                throw std::invalid_argument(
                    "Two ellipsis tags were encountered when processing "
                    "subscripts"
                );
                break;

            case dynamic_subscript::subscript_type::new_axis:
                head_ite = axes.insert(head_ite, make_phantom_axis());
                --last_subscript;
                break;

            case dynamic_subscript::subscript_type::index:
                apply_index(axis, offset, subscript.get_index());
                --last_subscript;
                --last_axis;
                break;
            
            case dynamic_subscript::subscript_type::slice:
                head_ite = axes.insert(head_ite, axis);
                apply_slice(*head_ite, offset, subscript.get_slice());
                --last_subscript;
                --last_axis;
                break;

            default:
                throw std::logic_error("Unknown subscript type encountered");
                break;
            }
        }
        
        if (first_subscript != last_subscript)
        {
            throw std::invalid_argument("Not all subscripts were processed");
        }

        // Copy reminder
        if (first_axis != last_axis)
        {
            axes.insert(head_ite, first_axis, last_axis);
        }
    }

};



class strided_layout::implementation
{
public:
    implementation(std::vector<strided_axis> &&axes,
                   std::ptrdiff_t offset ) noexcept
        : m_axes(std::move(axes))
        , m_offset(offset)
    {
    }

    implementation(const std::size_t *extents, 
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

    implementation(const std::size_t *extents, 
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

    std::size_t get_rank() const noexcept
    {
        return m_axes.size();
    }

    std::ptrdiff_t get_offset() const noexcept
    {
        return m_offset;
    }

    std::size_t compute_storage_requirement() const noexcept
    {
        std::size_t result = 0;

        bool zero_extent = false;
        for (const auto &axis : m_axes)
        {
            const auto extent = axis.get_extent();
            if (extent == 0)
            {
                zero_extent = true;
                break;
            }

            const auto stride = axis.get_stride();
            if(stride > 0)
            {
                const std::size_t last_index = extent - 1;
                result += last_index*stride;
            }
        }

        if (zero_extent)
        {
            result = 0;
        }
        else
        {
            ++result;
            result += m_offset;
        }

        return result;
    }

    std::size_t compute_element_count() const noexcept
    {
        return std::accumulate(
            m_axes.cbegin(), m_axes.cend(),
            std::size_t(1),
            [] (std::size_t current, const strided_axis &axis) -> std::size_t
            {
                return current * axis.get_extent();
            }
        );
    }

    implementation 
    apply_subscripts(span<const dynamic_subscript> subscripts) const
    {
        std::ptrdiff_t offset = m_offset;
        auto axes = apply_subscripts_helper::process(
            subscripts.begin(), subscripts.end(),
            m_axes.cbegin(), m_axes.cend(),
            offset
        );

        return implementation(std::move(axes), offset);
    }

    implementation transpose() const
    {
        std::vector<strided_axis> axes;
        axes.reserve(m_axes.size());

        std::reverse_copy(
            m_axes.cbegin(), m_axes.cend(),
            std::back_inserter(axes)
        );

        return implementation(std::move(axes), m_offset);
    }

    implementation permute(span<const std::size_t> order) const
    {
        const auto count = m_axes.size();
        check_axis_permutation(order.begin(), order.end(), count);

        std::vector<strided_axis> axes;
        axes.reserve(count);

        for(std::size_t i = 0; i < count; ++i)
        {
            axes.push_back(m_axes[order[i]]);
        }

        return implementation(std::move(axes), m_offset);
    }

    implementation swap_axes(std::ptrdiff_t axis1, std::ptrdiff_t axis2) const
    {
        const auto n = m_axes.size();
        const auto index1 = sanitize_index(axis1, n);
        const auto index2 = sanitize_index(axis2, n);

        auto axes = m_axes;
        std::swap(axes[index1], axes[index2]);

        return implementation(std::move(axes), m_offset);
    }

    implementation squeeze() const
    {
        std::vector<strided_axis> axes;

        std::copy_if(
            m_axes.cbegin(), m_axes.cend(),
            std::back_inserter(axes),
            is_significant
        );

        return implementation(std::move(axes), m_offset);
    }

    implementation broadcast_to(span<const std::size_t> extents) const
    {
        if (m_axes.size() > extents.size())
        {
            std::ostringstream oss;
            oss << "Can not broadcast layout with " << m_axes.size()
                << " axes into a shape of " << extents.size()
                << " dimensions.";
            throw std::invalid_argument(oss.str());
        }

        std::vector<strided_axis> axes;
        axes.reserve(extents.size());
        
        const std::size_t padding = extents.size() - m_axes.size();
        std::fill_n(
            std::back_inserter(axes), padding,
            make_phantom_axis()
        );
        std::copy(
            m_axes.cbegin(), m_axes.cend(),
            std::back_inserter(axes)
        );

        const auto count = extents.size();
        for(std::size_t i = 0; i < count; ++i)
        {
            auto &axis = axes[i];
            const auto extent = extents[i];
            if (!multidimensional::broadcast_axis_to_extent(axis, extent))
            {
                std::ostringstream oss;
                oss << "Can not broadcast axis of extent " << axis.get_extent()
                    << " into an extent of " << extent << ".";
                throw std::invalid_argument(oss.str());
            }
        }

        return implementation(std::move(axes), m_offset);
    }

private:
    std::vector<strided_axis> m_axes;
    std::ptrdiff_t m_offset;

};






XMIPP4_NODISCARD
std::size_t strided_layout::get_rank() const noexcept
{
    return m_implementation ? m_implementation->get_rank() : 0U;
}

XMIPP4_NODISCARD
std::ptrdiff_t strided_layout::get_offset() const noexcept
{
    return m_implementation ? m_implementation->get_offset() : 0U;
}

XMIPP4_NODISCARD
std::size_t strided_layout::compute_storage_requirement() const noexcept
{
    return m_implementation ? 
           m_implementation->compute_storage_requirement() : 
           0U ;
}

XMIPP4_NODISCARD
std::size_t strided_layout::compute_element_count() const noexcept
{
    return m_implementation ? m_implementation->compute_element_count() : 0U;
}

XMIPP4_NODISCARD
strided_layout 
strided_layout::apply_subscripts(span<const dynamic_subscript> subscripts) const
{
}

XMIPP4_NODISCARD
strided_layout strided_layout::transpose() const
{

}

XMIPP4_NODISCARD
strided_layout strided_layout::permute(span<const std::size_t> order) const
{

}

XMIPP4_NODISCARD
strided_layout 
strided_layout::swap_axes(std::ptrdiff_t axis1, std::ptrdiff_t axis2) const
{

}

XMIPP4_NODISCARD
strided_layout strided_layout::squeeze() const
{

}

XMIPP4_NODISCARD
strided_layout 
strided_layout::broadcast_to(span<const std::size_t> extents) const
{

}

} // namespace multidimensional
} // namespace xmipp4
