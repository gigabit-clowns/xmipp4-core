// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/fourier/fft_r2c_operation.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/complex_from_real_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

fft_r2c_operation::fft_r2c_operation() noexcept = default;

fft_r2c_operation::~fft_r2c_operation() = default;

std::string fft_r2c_operation::get_name() const
{
	return "fft_r2c";
}

std::size_t fft_r2c_operation::get_output_count() const noexcept { return 1; }
std::size_t fft_r2c_operation::get_input_count() const noexcept { return 1; }

const operation_shape_policy& 
fft_r2c_operation::get_operation_shape_policy() const noexcept
{
	// TODO
}

const operation_data_type_policy& 
fft_r2c_operation::get_operation_data_type_policy() const noexcept
{
	return complex_from_real_operation_data_type_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
