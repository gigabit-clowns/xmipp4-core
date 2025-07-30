// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_allocator.hpp"
#include "../numerical_type.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array;
class array_allocator;

std::shared_ptr<array> 
make_empty_array(numerical_type data_type,
                 std::size_t count, 
                 std::size_t *extents,
                 std::ptrdiff_t *strides = nullptr,
                 array_allocator& allocator = get_default_array_allocator() );

std::shared_ptr<array> 
make_zeros_array(numerical_type data_type,
                 std::size_t count, 
                 std::size_t *extents,
                 std::ptrdiff_t *strides = nullptr,
                 array_allocator& allocator = get_default_array_allocator() );

std::shared_ptr<array> 
make_ones_array(numerical_type data_type,
                std::size_t count, 
                std::size_t *extents,
                std::ptrdiff_t *strides = nullptr,
                array_allocator& allocator = get_default_array_allocator() );

} // namespace multidimensional
} // namespace xmipp4
