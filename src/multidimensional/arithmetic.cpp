// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/arithmetic.hpp>

#include <xmipp4/core/multidimensional/context.hpp>
#include <xmipp4/core/multidimensional/allocator.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/kernel_iteration_layout.hpp>
#include <xmipp4/core/multidimensional/kernel_key.hpp>
#include <xmipp4/core/multidimensional/kernel_registry.hpp>
#include <xmipp4/core/multidimensional/kernel_builder.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array add(const array &a, const array &b, const context &ctx, array *out)
{
    array result;

    std::array<strided_layout, 3> layouts;
    std::array<numerical_type, 3> data_types;
    std::vector<std::size_t> extents;

    if (out)
    {
        result = out->view();

        auto result_layout = result.get_layout();
        result_layout.get_extents(extents);

        layouts = {
            std::move(result_layout),
            a.get_layout().broadcast_to(make_span(extents)),
            b.get_layout().broadcast_to(make_span(extents))
        };
        data_types = {
            result.get_data_type(),
            a.get_data_type(),
            b.get_data_type()
        };

        // TODO check all in the same device.

    }
    else
    {
        layouts[1] = a.get_layout();
        layouts[2] = b.get_layout();
        data_types = {
            common_type(a.get_data_type(), b.get_data_type()),
            a.get_data_type(),
            b.get_data_type()
        };

        broadcast(
            span<strided_layout>(layouts.begin() + 1, layouts.end()),
            extents
        );

        // TODO compute result layout using ctx.get_layout_policy()

        result = array(
            layouts[0], 
            ctx.get_allocator()->allocate(0, 0, nullptr), // TODO
            data_types[0]
        );

        // TODO check all in the same device.
    }

    kernel_iteration_layout iteration_layout;
    std::vector<std::ptrdiff_t> strides;
    for (const auto &layout : layouts)
    {
        layout.get_strides(strides);
        iteration_layout.add_operand(
            extents, 
            std::move(strides), 
            layout.get_offset(), 
            layout.get_rank()
        );
    }

    const auto key = kernel_key::from_tag<void>(); // TODO determine. this is the only difference with multiply.
    kernel_registry registry; // TODO
    const auto builder = registry.get_kernel_builder(
        key, 
        result.get_storage()->get_backend()
    );

    // TODO check builder

    const auto kernel = builder->build(iteration_layout, make_span(data_types), ctx);

    std::array<storage *, 2> outputs = {
        result.get_storage()
    };
    std::array<const storage *, 2> inputs = {
        a.get_storage(),
        b.get_storage()
    };

    kernel->launch(
        make_span(outputs),
        make_span(inputs),
        ctx
    );

    return result;
}

array multiply(const array &a, const array &b, const context &ctx, array *out)
{
    // TODO similar to add
}

} // namespace multidimensional
} // namespace xmipp4
