// SPDX-License-Identifier: GPL-3.0-only

// pocketfft re-derives the twiddle factors of a transform every time it is
// asked for one unless it is allowed to remember them. A program is built
// once and run many times, so without this every run would pay for a plan it
// had already made. The cache is a bounded, mutex guarded set of function
// local statics inside pocketfft itself.
#define POCKETFFT_CACHE_SIZE 16

#include <backends/cpu/builders/fourier_transform.hpp>

#include <pocketfft_hdronly.h>

#include <complex>

namespace xmipp4
{
namespace cpu
{

namespace
{

bool is_forward(fourier_direction direction) noexcept
{
	return direction == fourier_direction::forward;
}

/**
 * @brief The factor a transform scales its result by.
 *
 * The forward transform is left as it is computed and the inverse divides by
 * the number of samples, which is what makes the two undo one another.
 */
template <typename T>
T get_scale(
	const fourier_layout_plan &plan,
	fourier_direction direction
) noexcept
{
	if (is_forward(direction))
	{
		return T(1);
	}

	// Never zero: an empty transformed axis is turned away when the plan is
	// made, so there is always at least the one sample of a transform along
	// no axis at all.
	return T(1) / static_cast<T>(plan.get_sample_count());
}

} // anonymous namespace

template <typename T>
void run_complex_to_complex_transform(
	const fourier_layout_plan &plan,
	fourier_direction direction,
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
		get_scale<T>(plan, direction)
	);
}

template <typename T>
void run_in_place_complex_transform(
	const fourier_layout_plan &plan,
	fourier_direction direction,
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
		get_scale<T>(plan, direction)
	);
}

template <typename T>
void run_real_to_complex_transform(
	const fourier_layout_plan &plan,
	fourier_direction direction,
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
		get_scale<T>(plan, direction)
	);
}

template <typename T>
void run_complex_to_real_transform(
	const fourier_layout_plan &plan,
	fourier_direction direction,
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
		get_scale<T>(plan, direction)
	);
}

// pocketfft is a header the size of a small library, and the transform is the
// one thing in this backend that does not have to be inlined into a caller:
// it is entered once per program run, not once per element. Instantiating it
// here is what keeps that header out of every builder that registers one.
template void run_complex_to_complex_transform<float32_t>(
	const fourier_layout_plan&,
	fourier_direction,
	std::complex<float32_t>*,
	const std::complex<float32_t>*
);
template void run_complex_to_complex_transform<float64_t>(
	const fourier_layout_plan&,
	fourier_direction,
	std::complex<float64_t>*,
	const std::complex<float64_t>*
);

template void run_in_place_complex_transform<float32_t>(
	const fourier_layout_plan&,
	fourier_direction,
	std::complex<float32_t>*
);
template void run_in_place_complex_transform<float64_t>(
	const fourier_layout_plan&,
	fourier_direction,
	std::complex<float64_t>*
);

template void run_real_to_complex_transform<float32_t>(
	const fourier_layout_plan&,
	fourier_direction,
	std::complex<float32_t>*,
	const float32_t*
);
template void run_real_to_complex_transform<float64_t>(
	const fourier_layout_plan&,
	fourier_direction,
	std::complex<float64_t>*,
	const float64_t*
);

template void run_complex_to_real_transform<float32_t>(
	const fourier_layout_plan&,
	fourier_direction,
	float32_t*,
	const std::complex<float32_t>*
);
template void run_complex_to_real_transform<float64_t>(
	const fourier_layout_plan&,
	fourier_direction,
	float64_t*,
	const std::complex<float64_t>*
);

} // namespace cpu
} // namespace xmipp4
