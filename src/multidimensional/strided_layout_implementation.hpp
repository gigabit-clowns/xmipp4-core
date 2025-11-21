// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/dynamic_subscript.hpp>

#include "strided_axis.hpp"
#include "../config.hpp"

#include <boost/container/small_vector.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout_implementation
{
public:
    using strided_axis_vector_type = boost::container::small_vector<
        strided_axis, 
        XMIPP4_SMALL_AXIS_COUNT
    >;

    strided_layout_implementation() = default;

    strided_layout_implementation(
        strided_axis_vector_type &&axes,
        std::ptrdiff_t offset 
    );   
    
    bool operator==(const strided_layout_implementation &other) const noexcept;

    std::size_t get_rank() const noexcept;

    template <typename Vec>
    void get_extents(Vec &extents) const;
    template <typename Vec>
    void get_strides(Vec &strides) const;
    std::ptrdiff_t get_offset() const noexcept;
    std::size_t compute_storage_requirement() const noexcept;
    std::size_t compute_element_count() const noexcept;

    strided_layout_implementation
    apply_subscripts(span<const dynamic_subscript> subscripts) const;

    strided_layout_implementation transpose() const;

    strided_layout_implementation permute(span<const std::size_t> order) const;

    strided_layout_implementation matrix_transpose(
        std::ptrdiff_t axis1, 
        std::ptrdiff_t axis2
    ) const;

    strided_layout_implementation matrix_diagonal(
        std::ptrdiff_t axis1, 
        std::ptrdiff_t axis2
    ) const;

    strided_layout_implementation squeeze() const;

    bool extents_equal(span<const std::size_t> extents) const noexcept;

    strided_layout_implementation broadcast_to(
        span<const std::size_t> extents
    ) const;

private:
    strided_axis_vector_type m_axes;
    std::ptrdiff_t m_offset;

};

} // namespace multidimensional
} // namespace xmipp4

#include "strided_layout_implementation.hpp"
