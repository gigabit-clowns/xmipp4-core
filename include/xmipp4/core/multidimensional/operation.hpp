// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "kernel.hpp"
#include "kernel_access_layout.hpp"
#include "../numerical_type.hpp"
#include "../span.hpp"

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout;
class array;

class operation
{
public:
    operation(kernel_access_layout access_layout,
              std::shared_ptr<kernel> kernel );

    void launch(span<array> read_write_operands, 
                span<const array> read_only_operands, 
                const context &context );

    void allocate_output(
        span<array> read_write_operands,
        const context &context
    );

    void validate_operands(span<array> read_write_operands, 
                           span<const array> read_only_operands );

private:
    std::vector<strided_layout> m_layouts;
    kernel_access_layout m_access_layout;
    std::shared_ptr<kernel> m_kernel;

};

} // namespace multidimensional
} // namespace xmipp4

