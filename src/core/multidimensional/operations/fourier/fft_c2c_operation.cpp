// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/fourier/fft_c2c_operation.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/homogeneous_operation_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_complex_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

fft_c2c_operation::fft_c2c_operation(
	std::vector<std::size_t> axes,
	fft_direction direction,
	fft_normalization normalization
) noexcept
	: fourier_operation(std::move(axes), normalization)
	, m_direction(direction)
{
}

fft_c2c_operation::~fft_c2c_operation() = default;

std::string fft_c2c_operation::get_name() const
{
    return "fft_c2c";
}

operation_arity fft_c2c_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

const operation_shape_policy&
fft_c2c_operation::get_operation_shape_policy() const noexcept
{
	return homogeneous_operation_shape_policy::get();
}

const operation_data_type_policy&
fft_c2c_operation::get_operation_data_type_policy() const noexcept
{
	return homogeneous_complex_operation_data_type_policy::get();
}

fft_direction fft_c2c_operation::get_direction() const noexcept
{
	return m_direction;
}

} // namespace multidimensional
} // namespace xmipp4
