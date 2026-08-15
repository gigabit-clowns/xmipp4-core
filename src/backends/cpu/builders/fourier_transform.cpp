// SPDX-License-Identifier: GPL-3.0-only

#include <backends/cpu/builders/fourier_transform.hpp>

#include <backends/cpu/config.hpp>

#include <complex>

#define POCKETFFT_CACHE_SIZE XMIPP4_POCKETFFT_CACHE_SIZE
#include <pocketfft_hdronly.h>

namespace xmipp4
{
namespace cpu
{

namespace
{

bool is_forward(ops::fourier_direction direction) noexcept
{
	return direction == ops::fourier_direction::forward;
}

/**
 * @brief The factor a transform scales its result by.
 *
 * Derived by the operations rather than here, so that every backend
 * implementing them arrives at the same number. Computed in double and
 * rounded once into the type the transform runs in.
 */
template <typename T>
T get_scale(
	const fourier_layout_plan &plan,
	ops::fourier_direction direction,
	ops::fourier_normalization normalization
) noexcept
{
	return static_cast<T>(
		ops::get_fourier_scale(
			normalization,
			direction,
			plan.get_sample_count()
		)
	);
}

} // anonymous namespace

template <typename T>
void run_complex_to_complex_transform(
	const fourier_layout_plan &plan,
	ops::fourier_direction direction,
	ops::fourier_normalization normalization,
	std::complex<T> *output,
	const std::complex<T> *input
)
{
	pocketfft::c2c(
		plan.get_shape(),
		plan.get_input_strides(),
		plan.get_output_strides(),
		plan.get_axes(),
		is_forward(direction),
		input,
		output,
		get_scale<T>(plan, direction, normalization)
	);
}

template <typename T>
void run_in_place_complex_transform(
	const fourier_layout_plan &plan,
	ops::fourier_direction direction,
	ops::fourier_normalization normalization,
	std::complex<T> *data
)
{
	pocketfft::c2c(
		plan.get_shape(),
		plan.get_output_strides(),
		plan.get_output_strides(),
		plan.get_axes(),
		is_forward(direction),
		data,
		data,
		get_scale<T>(plan, direction, normalization)
	);
}

template <typename T>
void run_real_to_complex_transform(
	const fourier_layout_plan &plan,
	ops::fourier_direction direction,
	ops::fourier_normalization normalization,
	std::complex<T> *output,
	const T *input
)
{
	pocketfft::r2c(
		plan.get_shape(),
		plan.get_input_strides(),
		plan.get_output_strides(),
		plan.get_axes(),
		is_forward(direction),
		input,
		output,
		get_scale<T>(plan, direction, normalization)
	);
}

template <typename T>
void run_complex_to_real_transform(
	const fourier_layout_plan &plan,
	ops::fourier_direction direction,
	ops::fourier_normalization normalization,
	T *output,
	const std::complex<T> *input
)
{
	pocketfft::c2r(
		plan.get_shape(),
		plan.get_input_strides(),
		plan.get_output_strides(),
		plan.get_axes(),
		is_forward(direction),
		input,
		output,
		get_scale<T>(plan, direction, normalization)
	);
}

// pocketfft is a header the size of a small library, and the transform is the
// one thing in this backend that does not have to be inlined into a caller:
// it is entered once per program run, not once per element. Instantiating it
// here is what keeps that header out of every builder that registers one.
template void run_complex_to_complex_transform<float32_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float32_t>*,
	const std::complex<float32_t>*
);
template void run_complex_to_complex_transform<float64_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float64_t>*,
	const std::complex<float64_t>*
);

template void run_in_place_complex_transform<float32_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float32_t>*
);
template void run_in_place_complex_transform<float64_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float64_t>*
);

template void run_real_to_complex_transform<float32_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float32_t>*,
	const float32_t*
);
template void run_real_to_complex_transform<float64_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float64_t>*,
	const float64_t*
);

template void run_complex_to_real_transform<float32_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	float32_t*,
	const std::complex<float32_t>*
);
template void run_complex_to_real_transform<float64_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	float64_t*,
	const std::complex<float64_t>*
);

} // namespace cpu
} // namespace xmipp4
