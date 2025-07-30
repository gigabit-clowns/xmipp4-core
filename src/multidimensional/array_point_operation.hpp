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

template <std::size_t N, typename F>
array& 
point_operation(const std::array<const array&, N> &inputs, 
                F &&operation, 
                array &output );

template <std::size_t N, typename F>
std::shared_ptr<array>
point_operation(const std::array<const array&, N> &inputs, 
                F &&operation, 
                array_allocator &allocator );


} // namespace multidimensional
} // namespace xmipp4
