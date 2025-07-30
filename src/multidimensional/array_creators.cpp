// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array_creators.hpp>

#include <xmipp4/core/multidimensional/array_allocator.hpp>
#include <xmipp4/core/multidimensional/array_operations.hpp>

namespace xmipp4 
{
namespace multidimensional
{

std::shared_ptr<array> 
make_empty_array(numerical_type data_type,
                 std::size_t count, 
                 std::size_t *extents,
                 std::ptrdiff_t *strides,
                 array_allocator& allocator )
{
    return allocator.allocate_array(data_type, count, extents, strides);
}

std::shared_ptr<array> 
make_zeros_array(numerical_type data_type,
                 std::size_t count, 
                 std::size_t *extents,
                 std::ptrdiff_t *strides,
                 array_allocator& allocator )
{
    const auto array = allocator.allocate_array(
        data_type, 
        count, 
        extents, 
        strides
    );

    if (array)
    {
        fill_zeros(*array);
    }

    return array;
}

std::shared_ptr<array> 
make_ones_array(numerical_type data_type,
                std::size_t count, 
                std::size_t *extents,
                std::ptrdiff_t *strides,
                array_allocator& allocator )
{
    const auto array = allocator.allocate_array(
        data_type, 
        count, 
        extents, 
        strides
    );

    if (array)
    {
        fill_ones(*array);
    }

    return array;
}

} // namespace multidimensional
} // namespace xmipp4
