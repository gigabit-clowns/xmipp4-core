// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_shape_policies/c2r_fft_operation_shape_policy.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include <sstream>
#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

c2r_fft_operation_shape_policy::c2r_fft_operation_shape_policy(
    std::size_t transformed_axis,
    bool is_output_odd
) noexcept
    : m_transformed_axis(transformed_axis)
    , m_is_output_odd(is_output_odd)
{
}

c2r_fft_operation_shape_policy::~c2r_fft_operation_shape_policy() = default;

std::size_t c2r_fft_operation_shape_policy::get_transformed_axis()
    const noexcept
{
    return m_transformed_axis;
}

bool c2r_fft_operation_shape_policy::is_output_odd() const noexcept
{
    return m_is_output_odd;
}

void c2r_fft_operation_shape_policy::deduce(
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
        oss << "c2r_fft_operation_shape_policy: transformed_axis "
            << m_transformed_axis
            << " is out of range for input with rank "
            << rank;
        throw std::invalid_argument(oss.str());
    }

    shape_type output = input;
    const auto m = input[m_transformed_axis];
    XMIPP4_ASSERT(m > 0);
    output[m_transformed_axis] =
        (m - 1) * 2 + static_cast<std::size_t>(m_is_output_odd);
    canonical_output_shapes[0] = std::move(output);
}

} // namespace multidimensional
} // namespace xmipp4
