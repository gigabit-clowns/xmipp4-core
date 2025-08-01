// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class array;
class array_allocator;

array
make_empty_array(numerical_type data_type,
                 /*TBD*/
                 array_allocator& allocator = get_default_array_allocator() );

array
make_zeros_array(numerical_type data_type,
                 /*TBD*/
                 array_allocator& allocator = get_default_array_allocator() );

array
make_ones_array(numerical_type data_type,
                /*TBD*/
                array_allocator& allocator = get_default_array_allocator() );

} // namespace multidimensional
} // namespace xmipp4
