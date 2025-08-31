// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/arithmetic.hpp>

#include <xmipp4/core/multidimensional/context.hpp>
#include <xmipp4/core/multidimensional/allocator.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>

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
    }

    // TODO build iteration layout
    // TODO get kernel and launch it

}

array multiply(const array &a, const array &b, const context &ctx, array *out)
{
    // TODO similar to add
}

} // namespace multidimensional
} // namespace xmipp4
