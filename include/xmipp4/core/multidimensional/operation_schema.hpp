// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "kernel_access_layout.hpp"
#include "../numerical_type.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class layout_policy;
class strided_layout;
class kernel_access_layout_builder;

class operation_schema
{
public:
    virtual ~operation_schema() = default;

    virtual std::size_t get_number_of_outputs() const noexcept = 0;
    virtual std::size_t get_number_of_inputs() const noexcept = 0;

    virtual 
    void deduce_output(span<numerical_type> output_numerical_types, 
                       span<strided_layout> output_layouts,
                       span<const numerical_type> input_numerical_types,
                       span<const strided_layout> input_layouts,
                       const layout_policy &policy  ) const = 0;

    virtual 
    void add_operand(
        kernel_access_layout_builder &builder, 
        const strided_layout &layout,
        numerical_type data_type 
    ) const = 0;

};

} // namespace multidimensional
} // namespace xmipp4
