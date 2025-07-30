// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <array>
#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array;
class array_allocator;

template <std::size_t N>
class point_operation
{
public:
    using input_array_references_type = std::array<const array&, N>;

    template <typename F>
    array& 
    operator()(const input_array_references_type &inputs, 
               F &&operation, 
               array &output );

    template <typename F>
    std::shared_ptr<array>
    operator()(const input_array_references_type &inputs, 
               F &&operation, 
               array_allocator &allocator );


};

} // namespace multidimensional
} // namespace xmipp4
