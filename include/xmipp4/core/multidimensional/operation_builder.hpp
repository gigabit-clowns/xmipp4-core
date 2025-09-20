// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation.hpp"
#include "kernel_access_layout_builder.hpp"
#include "../numerical_type.hpp"
#include "../span.hpp"

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

class operation_id;
class operation_schema;
class strided_layout;
class context;

class operation_builder
{
public:
    operation_builder& add_operand(
        const operation_schema &schema,
        const strided_layout &layout,
        numerical_type data_type
    );

    operation build(
        const operation_id &id,
        const compute::device_backend &backend,
        const context &context
    );

private:
    kernel_access_layout_builder m_access_layout_builder;

};

} // namespace multidimensional
} // namespace xmipp4
