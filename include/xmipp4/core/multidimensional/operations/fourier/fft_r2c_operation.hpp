// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/fourier_operation.hpp>
#include <xmipp4/core/multidimensional/operation_shape_policies/r2c_fft_operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <cstddef>
#include <vector>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Computes the direct Fast Fourier Transform (FFT) from real to complex
 * types.
 *
 * The transform is applied along all axes in @p axes. The output has the same
 * shape as the input except that the last axis in @p axes is compressed from
 * @c N to @c N/2+1 (half-spectrum). The output data type is the complex
 * equivalent of the input real type.
 */
class XMIPP4_CORE_API fft_r2c_operation final
	: public fourier_operation
{
public:
	/**
	 * @param axes Axes along which the FFT is computed. Must not be
	 * empty; the last axis is the one whose extent is compressed to @c N/2+1.
	 * @param normalization Normalization convention.
	 */
	explicit fft_r2c_operation(
		std::vector<std::size_t> axes,
		fft_normalization normalization = fft_normalization::none
	);
	~fft_r2c_operation() override;
	std::string get_name() const override;
	operation_arity get_arity() const noexcept override;
	const operation_shape_policy& 
	get_operation_shape_policy() const noexcept override;
	const operation_data_type_policy& 
	get_operation_data_type_policy() const noexcept override;

private:
	r2c_fft_operation_shape_policy m_shape_policy;
};

} // namespace multidimensional
} // namespace xmipp4
