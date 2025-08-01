// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "context.hpp"

namespace xmipp4 
{
namespace compute
{

class queue;

} // namespace compute
namespace multidimensional
{

array& add(const array &lhs, 
           const array &rhs, 
           array &output, 
           const context &context = get_default_context());

array& subtract(const array &lhs, 
                const array &rhs, 
                array &output,
                const context &context = get_default_context() );

array& multiply(const array &lhs, 
                const array &rhs, 
                array &output, 
                const context &context = get_default_context() );

array& divide(const array &lhs, 
              const array &rhs, 
              array &output, 
              const context &context = get_default_context() );

} // namespace multidimensional
} // namespace xmipp4
