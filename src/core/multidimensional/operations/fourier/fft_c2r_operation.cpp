// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/fourier/fft_c2r_operation.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/real_from_complex_operation_data_type_policy.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

fft_c2r_operation::fft_c2r_operation(
    std::vector<std::size_t> axes,
    bool is_output_odd,
    fft_normalization normalization
)
    : fourier_operation(std::move(axes), normalization)
    , m_shape_policy(get_axes().back(), is_output_odd)
{
}

fft_c2r_operation::~fft_c2r_operation() = default;

std::string fft_c2r_operation::get_name() const
{
    return "fft_c2r";
}

operation_arity fft_c2r_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

const operation_shape_policy&
fft_c2r_operation::get_operation_shape_policy() const noexcept
{
    return m_shape_policy;
}

const operation_data_type_policy&
fft_c2r_operation::get_operation_data_type_policy() const noexcept
{
    return real_from_complex_operation_data_type_policy::get();
}

bool fft_c2r_operation::is_output_odd() const noexcept
{
    return m_shape_policy.is_output_odd();
}

} // namespace multidimensional
} // namespace xmipp4
