// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_allocator.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array;



array& add(const array& lhs, const array& rhs, array &output);

std::shared_ptr<array> 
add(const array& lhs, 
    const array& rhs, 
    array_allocator &allocator = get_default_array_allocator());

array& subtract(const array& lhs, const array& rhs, array &output);

std::shared_ptr<array> 
subtract(const array& lhs, 
         const array& rhs, 
         array_allocator &allocator = get_default_array_allocator());

array& multiply(const array& lhs, const array& rhs, array &output);

std::shared_ptr<array> 
multiply(const array& lhs, 
         const array& rhs, 
         array_allocator &allocator = get_default_array_allocator());

array& divide(const array& lhs, const array& rhs, array &output);

std::shared_ptr<array> 
divide(const array& lhs, 
       const array& rhs, 
       array_allocator &allocator = get_default_array_allocator());

} // namespace multidimensional
} // namespace xmipp4
