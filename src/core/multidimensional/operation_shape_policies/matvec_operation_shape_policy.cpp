// SPDX-License-Identifier: GPL-3.0-only

#include "matvec_operation_shape_policy.hpp"

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include <stdexcept>
#include <algorithm>

namespace xmipp4
{
namespace multidimensional
{

void matvec_operation_shape_policy::infer_output(
    span<strided_layout> output_layouts,
    span<strided_layout> input_layouts
) const
{
    if (input_layouts.size() != 2)
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::infer_output: expected exactly 2 inputs."
        );
    }

    std::vector<std::size_t> mat_extents, vec_extents;
    input_layouts[0].get_extents(mat_extents);
    input_layouts[1].get_extents(vec_extents);

    if (mat_extents.size() != 2)
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::infer_output: input[0] (matrix) must be 2D."
        );
    }
    if (vec_extents.size() != 1)
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::infer_output: input[1] (vector) must be 1D."
        );
    }
    if (mat_extents[1] != vec_extents[0])
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::infer_output: matrix column count does not "
            "match vector length."
        );
    }

    const std::vector<std::size_t> out_extents = { mat_extents[0] };
    const auto out_layout =
        strided_layout::make_contiguous_layout(make_span(out_extents));
    std::fill(output_layouts.begin(), output_layouts.end(), out_layout);
}

void matvec_operation_shape_policy::validate(
    span<const strided_layout> output_layouts,
    span<strided_layout> input_layouts
) const
{
    if (output_layouts.empty())
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::validate: expected at least 1 output."
        );
    }
    if (input_layouts.size() != 2)
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::validate: expected exactly 2 inputs."
        );
    }

    std::vector<std::size_t> out_extents, mat_extents, vec_extents;
    output_layouts[0].get_extents(out_extents);
    input_layouts[0].get_extents(mat_extents);
    input_layouts[1].get_extents(vec_extents);

    if (out_extents.size() != 1)
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::validate: output must be 1D."
        );
    }
    if (mat_extents.size() != 2)
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::validate: input[0] (matrix) must be 2D."
        );
    }
    if (vec_extents.size() != 1)
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::validate: input[1] (vector) must be 1D."
        );
    }
    if (mat_extents[0] != out_extents[0])
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::validate: matrix row count does not match "
            "output length."
        );
    }
    if (mat_extents[1] != vec_extents[0])
    {
        throw std::invalid_argument(
            "matvec_operation_shape_policy::validate: matrix column count does not match "
            "vector length."
        );
    }
}

const matvec_operation_shape_policy& matvec_operation_shape_policy::get() noexcept
{
    static const matvec_operation_shape_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
