// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include "strided_axis.hpp"

#include <algorithm>
#include <vector>
#include <numeric>

namespace xmipp4 
{
namespace multidimensional
{

template <typename ForwardIt>
static
void check_axis_permutation(ForwardIt first, ForwardIt last, std::size_t count)
{
    // Based on:
    // https://en.cppreference.com/w/cpp/algorithm/is_permutation
    // The former function could be re-used, provided that something
    // like boost::counting_iterator is available.

    if (std::distance(first, last) != static_cast<std::ptrdiff_t>(count))
    {
        throw std::invalid_argument(
            "Axis permutation's length does not match the required count"
        );
    }

    // Skip common prefix
    std::size_t i = 0;
    while (first != last && *first == i)
    {
        ++first;
        ++i;
    }

    // For the rest, check that it is a permutation
    while (i < count)
    {
        // Ensure the current value appears in the range.
        auto ite = std::find(first, last, i);
        if (ite == last)
        {
            std::ostringstream oss;
            oss << "Index " << i << " is missing in the axis permutation";
            throw std::invalid_argument(oss.str());
        }

        ++i;
    }
}

class apply_subscripts_helper
{
public:
    template <typename BidirIt1, typename BidirIt2>
    static
    std::vector<strided_axis> 
    process(BidirIt1 first_subscript, BidirIt1 last_subscript,
            BidirIt2 first_axis, BidirIt2 last_axis,
            std::ptrdiff_t &offset )
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
        while (first_subscript != last_subscript)
        {
            const auto &subscript = *first_subscript;

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
                check_non_empty_axes_for_index(first_axis, last_axis);
                apply_index(*first_axis, offset, subscript.get_index());
                ++first_subscript;
                ++first_axis;
                break;
            
            case dynamic_subscript::subscript_type::slice:
                check_non_empty_axes_for_slice(first_axis, last_axis);
                head_ite = axes.insert(head_ite, *first_axis);
                apply_slice(*head_ite, offset, subscript.get_slice());
                ++first_subscript;
                ++first_axis;
                ++head_ite;
                break;

            default:
                throw std::invalid_argument("Unknown subscript type encountered");

            }
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
        while (first_subscript != last_subscript)
        {
            const auto &subscript = *std::prev(last_subscript);

            switch (subscript.get_subscript_type())
            {
            case dynamic_subscript::subscript_type::ellipsis:
                throw std::invalid_argument(
                    "Two ellipsis tags were encountered when processing "
                    "subscripts"
                );

            case dynamic_subscript::subscript_type::new_axis:
                head_ite = axes.insert(head_ite, make_phantom_axis());
                --last_subscript;
                break;

            case dynamic_subscript::subscript_type::index:
                check_non_empty_axes_for_index(first_axis, last_axis);
                apply_index(*std::prev(last_axis), offset, subscript.get_index());
                --last_subscript;
                --last_axis;
                break;
            
            case dynamic_subscript::subscript_type::slice:
                check_non_empty_axes_for_slice(first_axis, last_axis);
                head_ite = axes.insert(head_ite, *std::prev(last_axis));
                apply_slice(*head_ite, offset, subscript.get_slice());
                --last_subscript;
                --last_axis;
                break;

            default:
                throw std::invalid_argument("Unknown subscript type encountered");

            }
        }

        // Copy reminder
        if (first_axis != last_axis)
        {
            axes.insert(head_ite, first_axis, last_axis);
        }
    }


    template <typename BidirIt>
    static
    void check_non_empty_axes(
        BidirIt first_axis, 
        BidirIt last_axis, 
        const char *subscript_type
    )
    {
        if (first_axis == last_axis)
        {
            std::ostringstream oss;
            oss << subscript_type 
                << " subscript was encountered, but there are "
                   "no more axes to process";
            throw std::invalid_argument(oss.str());
        }
    }

    template <typename BidirIt>
    static
    void check_non_empty_axes_for_slice(
        BidirIt first_axis, 
        BidirIt last_axis
    )
    {
        check_non_empty_axes(first_axis, last_axis, "A slice");
    }

    template <typename BidirIt>
    static
    void check_non_empty_axes_for_index(
        BidirIt first_axis, 
        BidirIt last_axis
    )
    {
        check_non_empty_axes(first_axis, last_axis, "An index");
    }

};



class strided_layout::implementation
{
public:
    implementation() = default;

    implementation(std::vector<strided_axis> &&axes,
                   std::ptrdiff_t offset ) noexcept
        : m_axes(std::move(axes))
        , m_offset(offset)
    {
    }

    bool operator==(const implementation &other) const noexcept
    {
        if (m_offset != other.m_offset)
        {
            return false;
        }

        if (m_axes.size() != other.m_axes.size())
        {
            return false;
        }

        const auto n = m_axes.size();
        for (std::size_t i = 0; i < n; ++i)
        {
            const auto &axis1 = m_axes[i];
            const auto &axis2 = other.m_axes[i];

            const auto extent1 = axis1.get_extent();
            const auto extent2 = axis2.get_extent();
            if (extent1 != extent2)
            {
                return false;
            }

            const auto stride1 = axis1.get_stride();
            const auto stride2 = axis2.get_stride();
            if (extent1 != 1 && stride1 != stride2)
            {
                return false;
            }
        }

        return true;
    }

    std::size_t get_rank() const noexcept
    {
        return m_axes.size();
    }

    void get_extents(std::vector<std::size_t> &extents) const
    {
        XMIPP4_ASSERT(extents.empty());
        extents.reserve(m_axes.size());
        for (const auto &axis : m_axes)
        {
            extents.push_back(axis.get_extent());
        }
    }

    void get_strides(std::vector<std::ptrdiff_t> &strides) const
    {
        XMIPP4_ASSERT(strides.empty());
        strides.reserve(m_axes.size());
        for (const auto &axis : m_axes)
        {
            strides.push_back(axis.get_stride());
        }
    }

    std::ptrdiff_t get_offset() const noexcept
    {
        return m_offset;
    }

    std::size_t compute_storage_requirement() const noexcept
    {
        std::size_t result = 0;

        for (const auto &axis : m_axes)
        {
            const auto extent = axis.get_extent();
            if (extent == 0)
            {
                return 0;
            }

            const auto stride = axis.get_stride();
            if(stride > 0)
            {
                const std::size_t last_index = extent - 1;
                result += last_index*stride;
            }
        }

        return m_offset + result + 1;
    }

    std::size_t compute_element_count() const noexcept
    {
        return std::accumulate(
            m_axes.cbegin(), m_axes.cend(),
            std::size_t(1),
            [] (std::size_t current, const strided_axis &axis)
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

    implementation matrix_transpose(std::ptrdiff_t axis1, std::ptrdiff_t axis2) const
    {
        const auto n = m_axes.size();
        const auto index1 = sanitize_index(axis1, n);
        const auto index2 = sanitize_index(axis2, n);

        auto axes = m_axes;
        std::swap(axes[index1], axes[index2]);

        return implementation(std::move(axes), m_offset);
    }

    implementation matrix_diagonal(std::ptrdiff_t axis1, std::ptrdiff_t axis2) const
    {
        const auto n = m_axes.size();
        auto index1 = sanitize_index(axis1, n);
        auto index2 = sanitize_index(axis2, n);
        if (axis1 == axis2)
        {
            throw std::invalid_argument(
                "axis1 and axis2 must represent different axes"
            );
        }

        if (index1 > index2)
        {
            std::swap(index1, index2); // Sort
        }

        std::vector<strided_axis> axes;
        axes.reserve(n - 1);
        XMIPP4_ASSERT(index1 < index2);
        std::copy(
            m_axes.cbegin(), 
            std::next(m_axes.cbegin(), index1),
            std::back_inserter(axes)
        );
        std::copy(
            std::next(m_axes.cbegin(), index1+1),
            std::next(m_axes.cbegin(), index2),
            std::back_inserter(axes)
        );
        std::copy(
            std::next(m_axes.cbegin(), index2+1),
            std::next(m_axes.cend()),
            std::back_inserter(axes)
        );

        XMIPP4_ASSERT(axes.size() == n - 2);
        const auto &axis_a = m_axes[index1];
        const auto &axis_b = m_axes[index2];
        const auto extent = std::min(axis_a.get_extent(), axis_b.get_extent());
        const auto stride = axis_a.get_stride() + axis_b.get_stride();
        axes.emplace_back(extent, stride);
        
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

    bool extents_equal(span<const std::size_t> extents) const noexcept
    {
        if (m_axes.size() != extents.size())
        {
            return false;
        }

        const auto n = m_axes.size();
        for (std::size_t i = 0; i < n; ++i)
        {
            if (m_axes[i].get_extent() != extents[i])
            {
                return false;
            }
        }

        return true;
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
            if (!multidimensional::broadcast_to(axis, extent))
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



strided_layout::strided_layout() noexcept = default;
strided_layout::strided_layout(const strided_layout &other) noexcept = default;
strided_layout::strided_layout(strided_layout &&other) noexcept = default;
strided_layout::~strided_layout() = default;

strided_layout&
strided_layout::operator=(const strided_layout &other) noexcept = default;
strided_layout&
strided_layout::operator=(strided_layout &&other) noexcept = default;

strided_layout::strided_layout(
    std::shared_ptr<const implementation> impl
) noexcept
    : m_implementation(std::move(impl))
{
}

strided_layout::strided_layout(implementation &&impl)
    : m_implementation(std::make_shared<implementation>(std::move(impl)))
{
}


bool strided_layout::operator==(const strided_layout &other) const noexcept
{
    if (m_implementation == other.m_implementation)
    {
        return true; // Same or both null
    }
    else if (m_implementation && other.m_implementation)
    {
        return *m_implementation == *other.m_implementation;
    }
    else if (m_implementation)
    {
        return *m_implementation == implementation();
    }
    else // if (other.m_implementation)
    {
        XMIPP4_ASSERT(other.m_implementation);
        return implementation() == *other.m_implementation;
    }
}

bool strided_layout::operator!=(const strided_layout &other) const noexcept
{
    return !(*this == other);
}

XMIPP4_NODISCARD
std::size_t strided_layout::get_rank() const noexcept
{
    return m_implementation ? m_implementation->get_rank() : 0U;
}

void strided_layout::get_extents(std::vector<std::size_t> &extents) const
{
    extents.clear();
    if (m_implementation)
    {
        m_implementation->get_extents(extents);
    }
}

void strided_layout::get_strides(std::vector<std::ptrdiff_t> &strides) const
{
    strides.clear();
    if (m_implementation)
    {
        m_implementation->get_strides(strides);
    }
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
    if (subscripts.empty())
    {
        return *this; // No change
    }

    if (m_implementation)
    {
        const auto &impl = *m_implementation;
        return strided_layout(impl.apply_subscripts(subscripts));
    }
    else
    {
        return strided_layout(implementation().apply_subscripts(subscripts));
    }
}

XMIPP4_NODISCARD
strided_layout strided_layout::transpose() const
{
    if (get_rank() <= 1)
    {   
        return *this; // Empty or single axis. Not modified.
    }

    XMIPP4_ASSERT( m_implementation );
    const auto &impl = *m_implementation;
    return strided_layout(impl.transpose());
}

XMIPP4_NODISCARD
strided_layout strided_layout::permute(span<const std::size_t> order) const
{
    if (m_implementation)
    {
        const auto &impl = *m_implementation;
        return strided_layout(impl.permute(order));
    }
    else
    {
        implementation().permute(order); // Arg validation
        return strided_layout();
    }
}

XMIPP4_NODISCARD
strided_layout 
strided_layout::matrix_transpose(
    std::ptrdiff_t axis1, 
    std::ptrdiff_t axis2) const
{
    if (!m_implementation)
    {
        throw std::out_of_range("Cannot swap axes on an empty layout");
    }

    const auto &impl = *m_implementation;
    return strided_layout(impl.matrix_transpose(axis1, axis2));
}

XMIPP4_NODISCARD
strided_layout 
strided_layout::matrix_diagonal(
    std::ptrdiff_t axis1, 
    std::ptrdiff_t axis2
) const
{
    if (!m_implementation)
    {
        throw std::out_of_range(
            "Cannot call matrix_diagonal on an empty layout"
        );
    }

    const auto &impl = *m_implementation;
    return strided_layout(impl.matrix_diagonal(axis1, axis2));
}

XMIPP4_NODISCARD
strided_layout strided_layout::squeeze() const
{
    if (m_implementation)
    {
        const auto &impl = *m_implementation;
        return strided_layout(impl.squeeze());
    }
    else
    {
        return strided_layout();
    }
}

XMIPP4_NODISCARD
strided_layout 
strided_layout::broadcast_to(span<const std::size_t> extents) const
{
    if (m_implementation)
    {
        const auto &impl = *m_implementation;
        if (impl.extents_equal(extents))
        {
            return *this; // Re-use
        }
        else
        {
            return strided_layout(impl.broadcast_to(extents));
        }
    }
    else if(extents.empty())
    {
        return strided_layout();
    }
    else
    {
        return strided_layout(implementation().broadcast_to(extents));
    }

}

XMIPP4_NODISCARD
strided_layout strided_layout::make_contiguous_layout(
    span<const std::size_t> extents
)
{
    if (extents.empty())
    {
        return strided_layout(); // Empty layout
    }

    std::vector<strided_axis> axes;
    axes.reserve(extents.size());
    std::transform(
        extents.begin(), extents.end(),
        std::back_inserter(axes),
        [] (std::size_t extent)
        {
            return strided_axis(extent, 0);
        }
    );

    std::ptrdiff_t stride = 1;
    for (auto ite = axes.rbegin(); ite != axes.rend(); ++ite)
    {
        ite->set_stride(stride);
        stride *= ite->get_extent();
    }

    return strided_layout(implementation(std::move(axes), 0));
}

XMIPP4_NODISCARD
strided_layout strided_layout::make_custom_layout(
    span<const std::size_t> extents, 
    span<const std::ptrdiff_t> strides, 
    std::ptrdiff_t offset
)
{
    if (extents.size() != strides.size())
    {
        throw std::invalid_argument(
            "Extents and strides must have the same number of elements"
        );
    }
    
    const auto count = extents.size();
    if (count == 0 && offset == 0)
    {
        return strided_layout(); // Empty layout
    }

    std::vector<strided_axis> axes;
    axes.reserve(count);
    for (std::size_t i = 0; i < count; ++i)
    {
        axes.emplace_back(extents[i], strides[i]);
    }

    return strided_layout(implementation(std::move(axes), offset));
}

} // namespace multidimensional
} // namespace xmipp4
