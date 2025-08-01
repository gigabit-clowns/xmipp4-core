// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "context.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class array;
class array_allocator;

array
make_empty_array(numerical_type data_type,
                 /*TBD*/
                const context &context = get_default_context() );

array
make_zeros_array(numerical_type data_type,
                 /*TBD*/
                 const context &context = get_default_context() );

array
make_ones_array(numerical_type data_type,
                /*TBD*/
                const context &context = get_default_context() );

} // namespace multidimensional
} // namespace xmipp4
