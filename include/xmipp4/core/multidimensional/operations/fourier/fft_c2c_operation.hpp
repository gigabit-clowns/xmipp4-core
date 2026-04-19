// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/fourier_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Directions for the Fourier transform.
 */
enum class fft_direction
{
	forward, ///< Direct Fourier transform.
	backward ///< Inverse Fourier transform.
};

/**
 * @brief Computes the Fast Fourier Transform (FFT) from complex to complex 
 * types.
 */
class XMIPP4_CORE_API fft_c2c_operation final
	: public fourier_operation
{
public:
	explicit fft_c2c_operation(fft_direction direction) noexcept;
	~fft_c2c_operation() override;

	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
    const operation_shape_policy& get_operation_shape_policy() const noexcept override;
    const operation_data_type_policy& get_operation_data_type_policy() const noexcept override;

private:
	fft_direction m_direction;
};

} // namespace multidimensional
} // namespace xmipp4
