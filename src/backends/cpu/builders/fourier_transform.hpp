// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <backends/cpu/builders/fourier_layout_plan.hpp>
#include <backends/cpu/load_store.hpp>
#include <backends/cpu/loops/elementwise_loop.hpp>

#include <xmipp4/core/numerical/fixed_width_float.hpp>

#include <complex>
#include <type_traits>

namespace xmipp4
{
namespace cpu
{

/**
 * @brief Which way round a transform goes.
 *
 * The forward transform is left as it is computed and the inverse is divided
 * by the number of samples that reached each of its values, which is the
 * convention that makes the two undo one another and the one NumPy, SciPy and
 * the array API all take when no normalisation is named. None of the
 * operations carries a normalisation parameter, so this is the whole of the
 * matter.
 */
enum class fourier_direction
{
	forward,
	inverse
};

/**
 * @brief Whether this backend can transform elements of a storage type.
 *
 * The transform is delegated to pocketfft, which computes in single or double
 * precision. Half precision has neither: it is a storage type that this
 * backend widens to compute with, and a transform cannot be widened element by
 * element the way an elementwise kernel can, so it would have to be staged
 * through a whole array of its own. Nothing asks for that yet, and a spectrum
 * of any size leaves half precision's range long before it stops being
 * accurate, so the backend reports it unsupported instead of pretending.
 *
 * @tparam T The storage element type.
 */
template <typename T>
struct is_fourier_transformable
	: std::false_type
{
};

template <>
struct is_fourier_transformable<float32_t>
	: std::true_type
{
};

template <>
struct is_fourier_transformable<float64_t>
	: std::true_type
{
};

template <typename T>
struct is_fourier_transformable<std::complex<T>>
	: is_fourier_transformable<T>
{
};

/**
 * @brief Support predicate narrowing a Fourier builder to what pocketfft has.
 *
 * Every Fourier typing rule pivots on the single operand whose type is free,
 * so one pivot is all this ever sees.
 *
 * @see is_fourier_transformable
 */
template <typename... Pivots>
struct fourier_transform_support;

template <typename Pivot>
struct fourier_transform_support<Pivot>
	: is_fourier_transformable<Pivot>
{
};

/**
 * @brief Transform a complex operand into another one.
 *
 * @tparam T The component type. Instantiated for @ref float32_t and
 * @ref float64_t.
 * @param plan The planned transform.
 * @param direction Which way round to transform.
 * @param output The transformed operand, displaced by its offset.
 * @param input The operand to transform, displaced by its offset.
 */
template <typename T>
void run_complex_to_complex_transform(
	const fourier_layout_plan &plan,
	fourier_direction direction,
	std::complex<T> *output,
	const std::complex<T> *input
);

/**
 * @brief Transform a complex operand where it lies.
 *
 * How a real signal is transformed into a full spectrum: it is widened into
 * the output first, and what is transformed is then the output itself. The
 * operand is described by the output strides on both sides, which is what
 * pocketfft asks of a transform reading and writing the same storage.
 *
 * @tparam T The component type. Instantiated for @ref float32_t and
 * @ref float64_t.
 * @param plan The planned transform.
 * @param direction Which way round to transform.
 * @param data The operand, displaced by the output offset.
 */
template <typename T>
void run_in_place_complex_transform(
	const fourier_layout_plan &plan,
	fourier_direction direction,
	std::complex<T> *data
);

/**
 * @brief Transform a real operand into a half spectrum.
 *
 * @tparam T The component type. Instantiated for @ref float32_t and
 * @ref float64_t.
 * @param plan The planned transform.
 * @param direction Which way round to transform.
 * @param output The half spectrum, displaced by its offset.
 * @param input The real operand, displaced by its offset.
 */
template <typename T>
void run_real_to_complex_transform(
	const fourier_layout_plan &plan,
	fourier_direction direction,
	std::complex<T> *output,
	const T *input
);

/**
 * @brief Transform a half spectrum back into a real operand.
 *
 * @tparam T The component type. Instantiated for @ref float32_t and
 * @ref float64_t.
 * @param plan The planned transform.
 * @param direction Which way round to transform.
 * @param output The real operand, displaced by its offset.
 * @param input The half spectrum, displaced by its offset.
 */
template <typename T>
void run_complex_to_real_transform(
	const fourier_layout_plan &plan,
	fourier_direction direction,
	T *output,
	const std::complex<T> *input
);

namespace detail
{

struct fourier_conversion_kernel
{
	template <typename T, typename Q>
	void operator()(T *destination, const Q *source) const noexcept
	{
		cast(destination, source);
	}
};

} // namespace detail

/**
 * @brief Write an operand into another one, converting element by element.
 *
 * A real element becomes a complex one with no imaginary part, and a complex
 * element is copied across. Both operands must share their extents, so this
 * only serves a transform between two complex arrays.
 *
 * @tparam T The destination element type.
 * @tparam Q The source element type.
 * @param plan The planned transform.
 * @param output The destination, displaced by its offset.
 * @param input The source, displaced by its offset.
 */
template <typename T, typename Q>
void run_fourier_conversion(
	const fourier_layout_plan &plan,
	T *output,
	const Q *input
)
{
	run_elementwise_loop(
		detail::fourier_conversion_kernel(),
		plan.get_conversion_layout(),
		output,
		input
	);
}

/**
 * @brief Transform between two complex operands of the same extents.
 *
 * Serves both the forward transform and its inverse, which differ in the
 * direction alone, and admits the real operand the forward transform also
 * takes.
 *
 * @tparam Direction Which way round to transform.
 */
template <fourier_direction Direction>
struct complex_fourier_transform
{
	static fourier_transform_kind get_kind() noexcept
	{
		return fourier_transform_kind::complex_to_complex;
	}

	template <typename T>
	void operator()(
		const fourier_layout_plan &plan,
		std::complex<T> *output,
		const std::complex<T> *input
	) const
	{
		if (plan.get_axes().empty())
		{
			// A transform along no axis at all is the identity, which
			// pocketfft has no way of being asked for: it writes an output
			// once per axis, so with no axis it would write none.
			run_fourier_conversion(plan, output, input);
		}
		else
		{
			run_complex_to_complex_transform(
				plan,
				Direction,
				output,
				input
			);
		}
	}

	template <typename T>
	void operator()(
		const fourier_layout_plan &plan,
		std::complex<T> *output,
		const T *input
	) const
	{
		// A real signal is not a complex array anything can be read out of,
		// so it is widened into the output and transformed there. The pass
		// costs one write of the output, against the whole second half of a
		// spectrum that would otherwise have to be mirrored into place.
		run_fourier_conversion(plan, output, input);

		if (!plan.get_axes().empty())
		{
			run_in_place_complex_transform(plan, Direction, output);
		}
	}
};

/// Transform a real operand into the half spectrum a real signal needs.
struct real_to_complex_fourier_transform
{
	static fourier_transform_kind get_kind() noexcept
	{
		return fourier_transform_kind::real_to_complex;
	}

	template <typename T>
	void operator()(
		const fourier_layout_plan &plan,
		std::complex<T> *output,
		const T *input
	) const
	{
		run_real_to_complex_transform(
			plan,
			fourier_direction::forward,
			output,
			input
		);
	}
};

/// Transform a half spectrum back into the real signal it came from.
struct complex_to_real_fourier_transform
{
	static fourier_transform_kind get_kind() noexcept
	{
		return fourier_transform_kind::complex_to_real;
	}

	template <typename T>
	void operator()(
		const fourier_layout_plan &plan,
		T *output,
		const std::complex<T> *input
	) const
	{
		run_complex_to_real_transform(
			plan,
			fourier_direction::inverse,
			output,
			input
		);
	}
};

/// The forward transform of a complex signal.
using forward_complex_fourier_transform =
	complex_fourier_transform<fourier_direction::forward>;

/// The inverse transform of a complex spectrum.
using inverse_complex_fourier_transform =
	complex_fourier_transform<fourier_direction::inverse>;

} // namespace cpu
} // namespace xmipp4
