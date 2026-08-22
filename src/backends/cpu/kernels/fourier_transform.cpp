// SPDX-License-Identifier: GPL-3.0-only

#include <backends/cpu/kernels/fourier_transform.hpp>

#include <backends/cpu/config.hpp>

#include <complex>

// pocketfft parks its workers in a function local static whose destructor
// joins them. Compiled into this shared library, that join runs during static
// destruction, which on Windows is DLL_PROCESS_DETACH: after the loader has
// already stopped every other thread. MinGW's pthread_join waits there for a
// completion that never comes, and the process hangs at exit. It is the same
// reason cpu::device owns the pool this backend threads its own loops over
// rather than a static holding it.
//
// Nothing has hit this yet, only because pocketfft declines to thread a
// transform whose axis is shorter than 1000 samples and nothing asks it for a
// longer one. That is not a property worth relying on, so the threading is
// off until the transform is spread over cpu::thread_pool instead.
//
// This is the only translation unit that includes pocketfft, so defining it
// here cannot make one build of the header disagree with another.
#define POCKETFFT_NO_MULTITHREADING
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
	const std::complex<T> *input,
	std::size_t nthreads
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
		get_scale<T>(plan, direction, normalization),
		nthreads
	);
}

template <typename T>
void run_in_place_complex_transform(
	const fourier_layout_plan &plan,
	ops::fourier_direction direction,
	ops::fourier_normalization normalization,
	std::complex<T> *data,
	std::size_t nthreads
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
		get_scale<T>(plan, direction, normalization),
		nthreads
	);
}

template <typename T>
void run_real_to_complex_transform(
	const fourier_layout_plan &plan,
	ops::fourier_direction direction,
	ops::fourier_normalization normalization,
	std::complex<T> *output,
	const T *input,
	std::size_t nthreads
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
		get_scale<T>(plan, direction, normalization),
		nthreads
	);
}

template <typename T>
void run_complex_to_real_transform(
	const fourier_layout_plan &plan,
	ops::fourier_direction direction,
	ops::fourier_normalization normalization,
	T *output,
	const std::complex<T> *input,
	std::size_t nthreads
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
		get_scale<T>(plan, direction, normalization),
		nthreads
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
	const std::complex<float32_t>*,
	std::size_t
);
template void run_complex_to_complex_transform<float64_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float64_t>*,
	const std::complex<float64_t>*,
	std::size_t
);

template void run_in_place_complex_transform<float32_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float32_t>*,
	std::size_t
);
template void run_in_place_complex_transform<float64_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float64_t>*,
	std::size_t
);

template void run_real_to_complex_transform<float32_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float32_t>*,
	const float32_t*,
	std::size_t
);
template void run_real_to_complex_transform<float64_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	std::complex<float64_t>*,
	const float64_t*,
	std::size_t
);

template void run_complex_to_real_transform<float32_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	float32_t*,
	const std::complex<float32_t>*,
	std::size_t
);
template void run_complex_to_real_transform<float64_t>(
	const fourier_layout_plan&,
	ops::fourier_direction,
	ops::fourier_normalization,
	float64_t*,
	const std::complex<float64_t>*,
	std::size_t
);

} // namespace cpu
} // namespace xmipp4
