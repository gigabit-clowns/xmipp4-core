// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "strided_layout.hpp"

#include <xmipp4/core/multidimensional/array.hpp>

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
class array_implementation
    : public array
{
public:
    using value_type = T;

    array_implementation() = default;
    array_implementation(const array_implementation& other) = default;
    array_implementation(array_implementation&& other) = default;
    ~array_implementation() override = default;

    array_implementation& operator=(const array_implementation& other) = default;
    array_implementation& operator=(array_implementation&& other) = default;



    std::size_t get_rank() const noexcept override;
    
    std::size_t get_axis_parameters(std::size_t *extents,
                                    std::ptrdiff_t *strides,
                                    std::size_t count,
                                    std::size_t offset ) const noexcept override;

    const std::type_info& get_data_type() const noexcept override;

    numerical_type get_numeric_type() const noexcept override;

    XMIPP4_NODISCARD
    std::shared_ptr<view_type> 
    apply_subscripts(span<const dynamic_subscript> subscripts) override;

    XMIPP4_NODISCARD
    std::shared_ptr<const_view_type> 
    apply_subscripts(span<const dynamic_subscript> subscripts) const override;

    XMIPP4_NODISCARD
    std::shared_ptr<view_type> transpose() override;

    XMIPP4_NODISCARD
    std::shared_ptr<const_view_type> transpose() const override;

    XMIPP4_NODISCARD
    std::shared_ptr<view_type> 
    permute(span<const std::size_t> order) override;

    XMIPP4_NODISCARD
    std::shared_ptr<const_view_type> 
    permute(span<const std::size_t> order) const override;

    XMIPP4_NODISCARD
    std::shared_ptr<view_type> 
    swap_axes(std::size_t axis1, std::size_t axis2) override;

    XMIPP4_NODISCARD
    std::shared_ptr<const_view_type> 
    swap_axes(std::size_t axis1, std::size_t axis2) const override;

    XMIPP4_NODISCARD
    std::shared_ptr<view_type> squeeze() override;

    XMIPP4_NODISCARD
    std::shared_ptr<const_view_type> squeeze() const override;

    void 
    broadcast_extents_to_layout(std::vector<std::size_t> &extents,
                                std::size_t trailing_dimensions override) override;

    XMIPP4_NODISCARD
    std::shared_ptr<view_type> 
    broadcast_layout_to_extents(span<const std::size_t> extents,
                                std::size_t trailing_dimensions override ) override;

    XMIPP4_NODISCARD
    std::shared_ptr<const_view_type> 
    broadcast_layout_to_extents(span<const std::size_t> extents,
                                std::size_t trailing_dimensions override ) const override;

private:
    std::shared_ptr<strided_layout> m_layout;
    // TODO storage

};

} // namespace multidimensional
} // namespace xmipp4
