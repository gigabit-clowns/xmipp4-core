// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/fourier_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <cstddef>
#include <vector>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Computes the Fast Fourier Transform (FFT) from complex to complex
 * types.
 *
 * The transform is applied along all axes provided to the constructor. The
 * output has the same shape and data type as the input.
 */
class XMIPP4_CORE_API fft_c2c_operation final
	: public fourier_operation
{
public:
	/**
	 * @param axes Axes along which the FFT is computed. Must not be empty.
	 * @param direction Forward or backward transform.
	 * @param normalization Normalization convention.
	 */
	explicit fft_c2c_operation(
		std::vector<std::size_t> axes,
		fft_direction direction,
		fft_normalization normalization = fft_normalization::none
	) noexcept;
	~fft_c2c_operation() override;

	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
	const operation_shape_policy& 
	get_operation_shape_policy() const noexcept override;
	const operation_data_type_policy& 
	get_operation_data_type_policy() const noexcept override;

	/**
	 * @brief Return the transform direction.
	 */
	fft_direction get_direction() const noexcept;

private:
	fft_direction m_direction;
};

} // namespace multidimensional
} // namespace xmipp4
