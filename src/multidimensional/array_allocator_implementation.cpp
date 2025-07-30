// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_allocator_implementation.hpp"

#include <stdexcept>

namespace xmipp4 
{
namespace multidimensional
{

std::shared_ptr<array> 
array_allocator_implementation::allocate_array(numerical_type data_type,
                                               std::size_t count, 
                                               std::size_t *extents,
                                               std::ptrdiff_t *strides )
{
    auto layout = create_layout(count, extents, strides);
    const auto data_count = layout.compute_storage_requirement();
    // TODO
}   


strided_layout 
array_allocator_implementation::create_layout(std::size_t count, 
                                              std::size_t *extents,
                                              std::ptrdiff_t *strides )
{
    if(!extents)
    {
        throw std::invalid_argument("Extents must not be null.");
    }

    if (strides)
    {
        return strided_layout(extents, strides, count);
    }
    else
    {
        return strided_layout(extents, count);
    }
}

} // namespace multidimensional
} // namespace xmipp4
