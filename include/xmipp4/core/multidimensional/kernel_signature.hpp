// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class kernel_iteration_layout;
class strided_layout;
class layout_policy;

class kernel_signature
{
public:
    virtual ~kernel_signature() = default;
    
    virtual 
    void deduce_output_layout(span<const strided_layout> input_layouts, 
                              const layout_policy &policy,
                              std::vector<strided_layout>& output_layouts ) = 0;

    virtual 
    void build_kernel_layout(span<const strided_layout> layouts, 
                             kernel_iteration_layout &access_layout) = 0;

};

} // namespace multidimensional
} // namespace xmipp4

