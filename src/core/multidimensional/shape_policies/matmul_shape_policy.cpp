// SPDX-License-Identifier: GPL-3.0-only

#include "matmul_shape_policy.hpp"

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>

#include <stdexcept>
#include <algorithm>

namespace xmipp4
{
namespace multidimensional
{

void matmul_shape_policy::infer_output(
    span<strided_layout> output_layouts,
    span<strided_layout> input_layouts
) const
{
    if (input_layouts.size() != 2)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::infer_output: expected exactly 2 inputs."
        );
    }

    std::vector<std::size_t> extents0, extents1;
    input_layouts[0].get_extents(extents0);
    input_layouts[1].get_extents(extents1);

    if (extents0.size() < 2)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::infer_output: input[0] must have rank >= 2."
        );
    }
    if (extents1.size() < 2)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::infer_output: input[1] must have rank >= 2."
        );
    }

    const auto m  = extents0[extents0.size() - 2];
    const auto k0 = extents0[extents0.size() - 1];
    const auto k1 = extents1[extents1.size() - 2];
    const auto n  = extents1[extents1.size() - 1];

    if (k0 != k1)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::infer_output: inner dimensions do not match "
            "(input[0] last dim != input[1] second-to-last dim)."
        );
    }

    std::vector<std::size_t> batch0(extents0.begin(), extents0.end() - 2);
    std::vector<std::size_t> batch1(extents1.begin(), extents1.end() - 2);
    broadcast_extents(batch0, batch1);

    std::vector<std::size_t> out_extents = batch0;
    out_extents.push_back(m);
    out_extents.push_back(n);
    const auto out_layout =
        strided_layout::make_contiguous_layout(make_span(out_extents));
    std::fill(output_layouts.begin(), output_layouts.end(), out_layout);

    std::vector<std::size_t> new_extents0 = batch0;
    new_extents0.push_back(m);
    new_extents0.push_back(k0);
    input_layouts[0] = input_layouts[0].broadcast_to(make_span(new_extents0));

    std::vector<std::size_t> new_extents1 = batch0;
    new_extents1.push_back(k0);
    new_extents1.push_back(n);
    input_layouts[1] = input_layouts[1].broadcast_to(make_span(new_extents1));
}

void matmul_shape_policy::validate(
    span<const strided_layout> output_layouts,
    span<strided_layout> input_layouts
) const
{
    if (output_layouts.empty())
    {
        throw std::invalid_argument(
            "matmul_shape_policy::validate: expected at least 1 output."
        );
    }
    if (input_layouts.size() != 2)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::validate: expected exactly 2 inputs."
        );
    }

    std::vector<std::size_t> out_extents, extents0, extents1;
    output_layouts[0].get_extents(out_extents);
    input_layouts[0].get_extents(extents0);
    input_layouts[1].get_extents(extents1);

    if (out_extents.size() < 2)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::validate: output must have rank >= 2."
        );
    }
    if (extents0.size() < 2)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::validate: input[0] must have rank >= 2."
        );
    }
    if (extents1.size() < 2)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::validate: input[1] must have rank >= 2."
        );
    }

    const auto m = out_extents[out_extents.size() - 2];
    const auto n = out_extents[out_extents.size() - 1];

    if (extents0[extents0.size() - 2] != m)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::validate: input[0] second-to-last dim does "
            "not match output second-to-last dim (m)."
        );
    }

    const auto k = extents0[extents0.size() - 1];

    if (extents1[extents1.size() - 2] != k)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::validate: inner dimensions do not match "
            "(input[0] last dim != input[1] second-to-last dim)."
        );
    }
    if (extents1[extents1.size() - 1] != n)
    {
        throw std::invalid_argument(
            "matmul_shape_policy::validate: input[1] last dim does not match "
            "output last dim (n)."
        );
    }

    const std::vector<std::size_t> batch_out(
        out_extents.begin(), out_extents.end() - 2
    );

    std::vector<std::size_t> new_extents0 = batch_out;
    new_extents0.push_back(m);
    new_extents0.push_back(k);
    input_layouts[0] = input_layouts[0].broadcast_to(make_span(new_extents0));

    std::vector<std::size_t> new_extents1 = batch_out;
    new_extents1.push_back(k);
    new_extents1.push_back(n);
    input_layouts[1] = input_layouts[1].broadcast_to(make_span(new_extents1));
}

const matmul_shape_policy& matmul_shape_policy::get() noexcept
{
    static const matmul_shape_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
