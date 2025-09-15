// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "kernel_access_layout.hpp"
#include "../span.hpp"
#include "../numerical_type.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout;
class kernel_access_layout_implementation;

class kernel_access_layout_builder
{
public:
    kernel_access_layout_builder() noexcept;
    explicit kernel_access_layout_builder(std::vector<std::size_t> batch_extents);
    kernel_access_layout_builder(const kernel_access_layout_builder&) = delete;
    kernel_access_layout_builder(kernel_access_layout_builder&& other) noexcept;
    ~kernel_access_layout_builder();

    kernel_access_layout_builder& 
    operator=(const kernel_access_layout_builder&) = delete;
    kernel_access_layout_builder& 
    operator=(kernel_access_layout_builder&& other) noexcept;

    void add_operand(const strided_layout &layout,
                     numerical_type data_type, 
                     std::size_t core_dimensions );
    
    kernel_access_layout build();

private:
    std::unique_ptr<kernel_access_layout_implementation> m_implementation;

};

} // namespace multidimensional
} // namespace xmipp4
