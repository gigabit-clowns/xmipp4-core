// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_builder.hpp>

#include <xmipp4/core/multidimensional/operation_schema.hpp>
#include <xmipp4/core/multidimensional/kernel_builder.hpp>
#include <xmipp4/core/multidimensional/kernel_registry.hpp>
#include <xmipp4/core/multidimensional/context.hpp>

#include <stdexcept>

namespace xmipp4 
{
namespace multidimensional
{

operation_builder& operation_builder::add_operand(
    const operation_schema &schema,
    const strided_layout &layout,
    numerical_type data_type
)
{
    schema.add_operand(m_access_layout_builder, layout, data_type);
    return *this;
}

operation operation_builder::build(
    const operation_id &id,
    const compute::device_backend &backend,
    const context &context
)
{
    kernel_registry registry; // TODO obtain

    const auto *kernel_builder = registry.get_kernel_builder(id, backend);
    if (!kernel_builder)
    {
        std::terminate(); // TODO throw
    }

    auto access_layout = m_access_layout_builder.build();
    auto kernel = kernel_builder->build(access_layout, context);
    return operation(
        std::move(access_layout),
        std::move(kernel)
    );
}

} // namespace multidimensional
} // namespace xmipp4

