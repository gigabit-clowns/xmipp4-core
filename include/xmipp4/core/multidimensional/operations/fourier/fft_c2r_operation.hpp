// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/fourier_operation.hpp>
#include <xmipp4/core/multidimensional/operation_shape_policies/c2r_fft_operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <cstddef>
#include <vector>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Computes the Inverse Fast Fourier Transform (IFFT) from complex to
 * real types.
 *
 * The transform is applied along all axes in @p axes. The last axis in
 * @p axes is expanded from @c M = @c N/2+1 back to @c N, where @c N is
 * either @c 2*(M-1) (@p is_output_odd == false) or @c 2*M-1
 * (@p is_output_odd == true). The output data type is the real equivalent
 * of the input complex type.
 */
class XMIPP4_CORE_API fft_c2r_operation final
	: public fourier_operation
{
public:
	/**
	 * @param axes Axes along which the IFFT is computed. Must not be
	 * empty; the last axis is the one whose extent is expanded.
	 * @param is_output_odd True if the reconstructed real size along the last
	 * axis is odd (@c N = 2*M-1); false for even (@c N = 2*(M-1)).
	 * @param normalization Normalization convention.
	 */
	explicit fft_c2r_operation(
		std::vector<std::size_t> axes,
		bool is_output_odd,
		fft_normalization normalization = fft_normalization::none
	);
	~fft_c2r_operation() override;

	std::string get_name() const override;
	operation_arity get_arity() const noexcept override;
	const operation_shape_policy& 
	get_operation_shape_policy() const noexcept override;
	const operation_data_type_policy& 
	get_operation_data_type_policy() const noexcept override;

	/**
	 * @brief Return whether the real output size along the last axis is odd.
	 */
	bool is_output_odd() const noexcept;

private:
	c2r_fft_operation_shape_policy m_shape_policy;
};

} // namespace multidimensional
} // namespace xmipp4
