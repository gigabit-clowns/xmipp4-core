// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "default_array_allocator.hpp"
#include "default_array.hpp"

#include <stdexcept>

namespace xmipp4 
{
namespace multidimensional
{

std::shared_ptr<array> 
default_array_allocator::allocate_array(numerical_type data_type,
                                        std::size_t count, 
                                        std::size_t *extents,
                                        std::ptrdiff_t *strides )
{
    return create_array(
        data_type, 
        create_layout(count, extents, strides) 
    );
}   


std::shared_ptr<strided_layout>
default_array_allocator::create_layout(std::size_t count, 
                                       std::size_t *extents,
                                       std::ptrdiff_t *strides )
{
    if(!extents)
    {
        throw std::invalid_argument("Extents must not be null.");
    }

    std::shared_ptr<strided_layout> result;
    if (strides)
    {
        result = std::make_shared<strided_layout>(extents, strides, count);
    }
    else
    {
        result = std::make_shared<strided_layout>(extents, count);
    }

    return result;
}

} // namespace multidimensional
} // namespace xmipp4
