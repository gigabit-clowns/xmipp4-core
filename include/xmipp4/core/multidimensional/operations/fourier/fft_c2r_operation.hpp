// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/fourier_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Computes the Inverse Fast Fourier Transform (IFFT) from complex to 
 * real types.
 */
class XMIPP4_CORE_API fft_c2r_operation final
	: public fourier_operation
{
public:
	explicit fft_c2r_operation() noexcept;
	~fft_c2r_operation() override;

	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
    const operation_shape_policy& get_operation_shape_policy() const noexcept override;
    const operation_data_type_policy& get_operation_data_type_policy() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
