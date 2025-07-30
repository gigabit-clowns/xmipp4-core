// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "strided_layout.hpp"

#include <xmipp4/core/multidimensional/array_allocator.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class array_allocator_implementation
    : public array_allocator
{
public:
    array_allocator_implementation() = default;
    array_allocator_implementation(const array_allocator_implementation& other) = default;
    array_allocator_implementation(array_allocator_implementation&& other) = default;
    ~array_allocator_implementation() override = default;

    array_allocator_implementation& 
    operator=(const array_allocator_implementation& other) = default;
    array_allocator_implementation& 
    operator=(array_allocator_implementation&& other) = default;

    std::shared_ptr<array> 
    allocate_array(numerical_type data_type,
                   std::size_t count, 
                   std::size_t *extents,
                   std::ptrdiff_t *strides = nullptr ) override;

private:
    static strided_layout create_layout(std::size_t count, 
                                        std::size_t *extents,
                                        std::ptrdiff_t *strides );

};

} // namespace multidimensional
} // namespace xmipp4
