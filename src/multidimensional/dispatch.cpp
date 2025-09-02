// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/dispatch.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/context.hpp>
#include <xmipp4/core/multidimensional/operation_id.hpp>
#include <xmipp4/core/multidimensional/operation_schema.hpp>

namespace xmipp4 
{
namespace multidimensional
{

void dispatch(const operation_id &key,
              const operation_schema &schema, 
              span<array> outputs, 
              span<const array> inputs, 
              const context &ctx,
              bool deduce_output, 
              const_any_reference params )
{
    std::vector<std::size_t> extents;

    // Populate structures
    const std::size_t n_operands = outputs.size() + inputs.size();
    std::vector<strided_layout> layouts;
    std::vector<numerical_type> data_types;
    std::vector<storage *> output_storages;
    std::vector<const storage *> input_storages;
    layouts.reserve(n_operands);
    data_types.reserve(n_operands);
    output_storages.reserve(outputs.size());
    input_storages.reserve(inputs.size());

    for (auto &output : outputs)
    {
        layouts.push_back(output.get_layout());
        data_types.push_back(output.get_data_type());
        output_storages.push_back(output.get_storage());
    }
    for (const auto &input : inputs)
    {
        layouts.push_back(input.get_layout());
        data_types.push_back(input.get_data_type());
        input_storages.push_back(input.get_storage());
    }

    // Dispatch
    kernel_iteration_layout iteration_layout = 
        schema.build_iteration_layout(make_span(layouts));

    kernel_registry registry; // TODO
    const auto builder = registry.get_kernel_builder(
        key, 
        result.get_storage()->get_backend() // TODO
    );

    const auto kernel = builder->build(
        iteration_layout, 
        make_span(data_types), 
        ctx
    );
    kernel->launch(
        make_span(output_storages),
        make_span(input_storages),
        ctx
    );

    return result;
}

} // namespace multidimensional
} // namespace xmipp4

