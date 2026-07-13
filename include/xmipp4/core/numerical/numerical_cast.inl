// SPDX-License-Identifier: GPL-3.0-only

#include "numerical_cast.hpp"

#include "fixed_width_float.hpp"

namespace xmipp4
{

namespace detail
{

/**
 * @brief Trait detecting @c std::complex specializations.
 */
template <typename T>
struct is_complex : std::false_type {};

template <typename T>
struct is_complex<std::complex<T>> : std::true_type {};

/**
 * @brief Trait detecting scalar types that convert through @c float.
 *
 * Satisfied by non-arithmetic value types that are convertible to and
 * from @c float, such as @ref float16_t or half precision library types.
 */
template <typename T>
struct is_float_proxy
	: std::integral_constant<
		bool,
		!std::is_arithmetic<T>::value &&
		std::is_convertible<T, float>::value &&
		std::is_convertible<float, T>::value
	>
{
};

/**
 * @brief Implementation of @ref numerical_cast selected by partial
 * specialization.
 *
 * The primary template handles every pair that does not involve
 * a float proxy nor @c std::complex, deferring to a plain @c static_cast.
 */
template <typename T, typename Q, typename = void>
struct numerical_cast_helper
{
	static T cast(const Q &src) noexcept
	{
		return static_cast<T>(src);
	}
};

/**
 * @brief Conversion towards a float proxy type.
 *
 * Makes the source-to-@c float step explicit before building the proxy.
 */
template <typename T, typename Q>
struct numerical_cast_helper<
	T, Q,
	typename std::enable_if<
		is_float_proxy<T>::value && !is_float_proxy<Q>::value
	>::type
>
{
	static T cast(const Q &src) noexcept
	{
		return T(static_cast<float>(src));
	}
};

/**
 * @brief Conversion away from a float proxy type.
 *
 * Makes the @c float-to-destination step explicit after reading the
 * proxy.
 */
template <typename T, typename Q>
struct numerical_cast_helper<
	T, Q,
	typename std::enable_if<
		!is_float_proxy<T>::value && is_float_proxy<Q>::value &&
		!is_complex<T>::value
	>::type
>
{
	static T cast(const Q &src) noexcept
	{
		return static_cast<T>(static_cast<float>(src));
	}
};

/**
 * @brief Conversion between two distinct float proxy types.
 */
template <typename T, typename Q>
struct numerical_cast_helper<
	T, Q,
	typename std::enable_if<
		is_float_proxy<T>::value && is_float_proxy<Q>::value &&
		!std::is_same<T, Q>::value
	>::type
>
{
	static T cast(const Q &src) noexcept
	{
		return T(static_cast<float>(src));
	}
};

/**
 * @brief Conversion between two @c std::complex values.
 *
 * Applied element-wise so that @ref float16_t components go through the scalar
 * specializations above.
 */
template <typename T, typename Q>
struct numerical_cast_helper<std::complex<T>, std::complex<Q>>
{
	static std::complex<T> cast(const std::complex<Q> &src) noexcept
	{
		return std::complex<T>(
			numerical_cast<T>(src.real()),
			numerical_cast<T>(src.imag())
		);
	}
};

/**
 * @brief Conversion from a scalar into a @c std::complex value.
 *
 * The scalar becomes the real part; the imaginary part is left at zero. The
 * real part goes through the scalar conversion so that a @ref float16_t
 * component is handled explicitly.
 */
template <typename T, typename Q>
struct numerical_cast_helper<
	std::complex<T>, Q,
	typename std::enable_if<!is_complex<Q>::value>::type
>
{
	static std::complex<T> cast(const Q &src) noexcept
	{
		return std::complex<T>(numerical_cast<T>(src));
	}
};

} // namespace detail

template <typename T, typename Q>
XMIPP4_NODISCARD
T numerical_cast(const Q &src) noexcept
{
	return detail::numerical_cast_helper<T, Q>::cast(src);
}

} // namespace xmipp4
