// SPDX-License-Identifier: GPL-3.0-only

#include "outer_operation_shape_policy.hpp"

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include <stdexcept>
#include <algorithm>

namespace xmipp4
{
namespace multidimensional
{

void outer_operation_shape_policy::infer_output(
    span<strided_layout> output_layouts,
    span<strided_layout> input_layouts
) const
{
    if (input_layouts.size() != 2)
    {
        throw std::invalid_argument(
            "outer_operation_shape_policy::infer_output: expected exactly 2 inputs."
        );
    }

    std::vector<std::size_t> extents0, extents1;
    input_layouts[0].get_extents(extents0);
    input_layouts[1].get_extents(extents1);

    if (extents0.size() != 1)
    {
        throw std::invalid_argument(
            "outer_operation_shape_policy::infer_output: input[0] must be 1D."
        );
    }
    if (extents1.size() != 1)
    {
        throw std::invalid_argument(
            "outer_operation_shape_policy::infer_output: input[1] must be 1D."
        );
    }

    const std::vector<std::size_t> out_extents = { extents0[0], extents1[0] };
    const auto out_layout =
        strided_layout::make_contiguous_layout(make_span(out_extents));
    std::fill(output_layouts.begin(), output_layouts.end(), out_layout);
}

void outer_operation_shape_policy::validate(
    span<const strided_layout> output_layouts,
    span<strided_layout> input_layouts
) const
{
    if (output_layouts.empty())
    {
        throw std::invalid_argument(
            "outer_operation_shape_policy::validate: expected at least 1 output."
        );
    }
    if (input_layouts.size() != 2)
    {
        throw std::invalid_argument(
            "outer_operation_shape_policy::validate: expected exactly 2 inputs."
        );
    }

    std::vector<std::size_t> out_extents, extents0, extents1;
    output_layouts[0].get_extents(out_extents);
    input_layouts[0].get_extents(extents0);
    input_layouts[1].get_extents(extents1);

    if (out_extents.size() != 2)
    {
        throw std::invalid_argument(
            "outer_operation_shape_policy::validate: output must be 2D."
        );
    }
    if (extents0.size() != 1)
    {
        throw std::invalid_argument(
            "outer_operation_shape_policy::validate: input[0] must be 1D."
        );
    }
    if (extents1.size() != 1)
    {
        throw std::invalid_argument(
            "outer_operation_shape_policy::validate: input[1] must be 1D."
        );
    }
    if (extents0[0] != out_extents[0])
    {
        throw std::invalid_argument(
            "outer_operation_shape_policy::validate: input[0] length does not match "
            "output row count."
        );
    }
    if (extents1[0] != out_extents[1])
    {
        throw std::invalid_argument(
            "outer_operation_shape_policy::validate: input[1] length does not match "
            "output column count."
        );
    }
}

const outer_operation_shape_policy& outer_operation_shape_policy::get() noexcept
{
    static const outer_operation_shape_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
