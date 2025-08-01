// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"

namespace xmipp4 
{
namespace compute
{

class queue;

} // namespace compute
namespace multidimensional
{

class array;



array& add(const array& lhs, const array& rhs, array &output);

array add(const array& lhs, 
          const array& rhs, 
          array_allocator &allocator = get_default_array_allocator() );

array& subtract(const array& lhs, const array& rhs, array &output);

array subtract(const array& lhs, 
               const array& rhs, 
               array_allocator &allocator = get_default_array_allocator());

array& multiply(const array& lhs, const array& rhs, array &output);

array multiply(const array& lhs, 
               const array& rhs, 
               array_allocator &allocator = get_default_array_allocator());

array& divide(const array& lhs, const array& rhs, array &output);

array divide(const array& lhs, 
             const array& rhs, 
             array_allocator &allocator = get_default_array_allocator());

} // namespace multidimensional
} // namespace xmipp4
