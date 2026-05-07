// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_shape_policies/r2c_fft_operation_shape_policy.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include <sstream>
#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

r2c_fft_operation_shape_policy::r2c_fft_operation_shape_policy(
    std::size_t transformed_axis
) noexcept
    : m_transformed_axis(transformed_axis)
{
}

r2c_fft_operation_shape_policy::~r2c_fft_operation_shape_policy() = default;

std::size_t r2c_fft_operation_shape_policy::get_transformed_axis()
    const noexcept
{
    return m_transformed_axis;
}

void r2c_fft_operation_shape_policy::deduce(
    span<shape_type> canonical_output_shapes,
    span<const shape_type> input_shapes
) const
{
    XMIPP4_ASSERT(input_shapes.size() == 1);
    XMIPP4_ASSERT(canonical_output_shapes.size() == 1);

    const shape_type& input = input_shapes[0];
    const auto rank = input.size();

    if (m_transformed_axis >= rank)
    {
        std::ostringstream oss;
        oss << "r2c_fft_operation_shape_policy: transformed_axis "
            << m_transformed_axis
            << " is out of range for input with rank "
            << rank;
        throw std::invalid_argument(oss.str());
    }

    shape_type output = input;
    const auto n = input[m_transformed_axis];
    XMIPP4_ASSERT(n > 0);
    output[m_transformed_axis] = n / 2 + 1;
    canonical_output_shapes[0] = std::move(output);
}

} // namespace multidimensional
} // namespace xmipp4
