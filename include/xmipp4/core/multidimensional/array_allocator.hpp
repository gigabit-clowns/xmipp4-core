// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../numerical_type.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array;

class array_allocator
{
public:
    array_allocator() = default;
    array_allocator(const array_allocator& other) = default;
    array_allocator(array_allocator&& other) = default;
    virtual ~array_allocator() = default;

    array_allocator& operator=(const array_allocator& other) = default;
    array_allocator& operator=(array_allocator&& other) = default;

    virtual std::shared_ptr<array>
    allocate_array(numerical_type data_type,
                   std::size_t count, 
                   std::size_t *extents,
                   std::ptrdiff_t *strides = nullptr) = 0;

};



array_allocator& get_default_array_allocator() noexcept;

} // namespace multidimensional
} // namespace xmipp4
