// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "strided_layout.hpp"

#include <xmipp4/core/multidimensional/array_allocator.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class default_array_allocator
    : public array_allocator
{
public:
    default_array_allocator() = default;
    default_array_allocator(const default_array_allocator& other) = default;
    default_array_allocator(default_array_allocator&& other) = default;
    ~default_array_allocator() override = default;

    default_array_allocator& 
    operator=(const default_array_allocator& other) = default;
    default_array_allocator& 
    operator=(default_array_allocator&& other) = default;

    std::shared_ptr<array> 
    allocate_array(numerical_type data_type,
                   std::size_t count, 
                   std::size_t *extents,
                   std::ptrdiff_t *strides = nullptr ) override;

private:
    static 
    std::shared_ptr<strided_layout> 
    create_layout(std::size_t count, 
                  std::size_t *extents,
                  std::ptrdiff_t *strides );

    static 
    std::shared_ptr<array> 
    create_array(numerical_type data_type,
                 std::shared_ptr<strided_layout> layout );

};

} // namespace multidimensional
} // namespace xmipp4
